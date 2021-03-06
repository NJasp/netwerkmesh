#define Q_SZ 64

#include <stdio.h>

#include "shell.h"
#include "msg.h"
#include "thread.h"

#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/ipv6.h"

#define MAIN_QUEUE_SIZE     (8)

extern int udp_cmd(int argc, char **argv);
extern void start_server(char *port_str);

static kernel_pid_t led_pid;
static msg_t main_msg_q[Q_SZ];
static char _ledstack[THREAD_STACKSIZE_SMALL]; // No large stack size neccesary, so using small stack
static kernel_pid_t udp_pid;
static char _udp_receive_stack[THREAD_STACKSIZE_MAIN];
static char _stack[THREAD_STACKSIZE_MAIN];
static const shell_command_t shell_commands[] = {
    { "udp", "send data over UDP and listen on UDP ports", udp_cmd },
    { NULL, NULL, NULL }
};

// Give LED thread a kernel pid and a stack
// Function containing the code to be exectued in the LED thread
static void *_led_fwd_eventloop(void *arg)
{
    while(1) {
	// Write LED high, wait 1 second and write LED low
        gpio_write(GPIO4, 1);
        xtimer_sleep(1);
        gpio_write(GPIO4, 0);
	// Set thread to sleeping, when new message arrives the thread will be waken up
        thread_sleep();
    }
    /* never reached */
    return NULL;
}

//Scheduled task to receive udp packets
void *_udp_receive_loop(void *arg)
{
	//Variable for message queue used by application layer
    static msg_t _msg_q[Q_SZ];
	//Create a message to reply with
    msg_t msg, reply;
    reply.type = GNRC_NETAPI_MSG_TYPE_ACK;
    reply.content.value = -ENOTSUP;
	//Initialize the message queue with the static queue size
    msg_init_queue(_msg_q, Q_SZ);
	//Create a empty UDP packet pointer that will be used to point to the data of the udp packet 
    gnrc_pktsnip_t *pkt = NULL;
    gnrc_netreg_entry_t me_reg;
	//Setup the thread and port number that is used
    me_reg.demux_ctx = 88;
    me_reg.target.pid = thread_getpid();
	//Register this thread to be used as udp receive thread
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &me_reg);
    while (1) {
		//Listen on incoming messages
        msg_receive(&msg);
		//If a message is received, lookup the type of message
        switch (msg.type) {
			//Message is a incoming message
            case GNRC_NETAPI_MSG_TYPE_RCV:
				//Set the packet pointer to the data of the message
                pkt = msg.content.ptr;
				//Print the data of the packet
                printf("UDP message received: %s\n", (char*)pkt->data);
                break;
			//Message is a outgoing message
            case GNRC_NETAPI_MSG_TYPE_SND:
			    //Set the packet pointer to the data of the message
				pkt = msg.content.ptr;
				//Print the data of the packet
                printf("UDP message send: %s\n", (char*)pkt->data);
                break;
             case GNRC_NETAPI_MSG_TYPE_SET:
             case GNRC_NETAPI_MSG_TYPE_GET:
                msg_reply(&msg, &reply);
                break;
            default:
                break;
        }
    }
    return NULL;
}
#ifdef MODULE_GNRC_SIXLOWPAN
static void *_ipv6_fwd_eventloop(void *arg)
{
    (void)arg;

    msg_t msg, msg_q[8];
    gnrc_netreg_entry_t me_reg = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                                            thread_getpid());

    msg_init_queue(msg_q, 8);

    gnrc_netreg_register(GNRC_NETTYPE_SIXLOWPAN, &me_reg);

    while(1) {
        msg_receive(&msg);
        gnrc_pktsnip_t *pkt = msg.content.ptr;
        if(msg.type == GNRC_NETAPI_MSG_TYPE_SND) {
            gnrc_pktsnip_t *ipv6 = gnrc_pktsnip_search_type(pkt, GNRC_NETTYPE_IPV6);
            ipv6_addr_t addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
            int res;
            ipv6 = ipv6->data;

            ipv6_hdr_t *ipv6_hdr =(ipv6_hdr_t *)ipv6;

            /* get the first IPv6 interface and prints its address */
            gnrc_netif_t *netif = gnrc_netif_iter(NULL);
            res = gnrc_netif_ipv6_addrs_get(netif, addrs, sizeof(addrs));
            if (res < 0) {
                /* an error occurred, just continue */
                continue;
            }
            for (unsigned i = 0; i < (res / sizeof(ipv6_addr_t)); i++) {
                if ((!ipv6_addr_is_link_local(&addrs[i])) &&
                    (!ipv6_addr_is_link_local(&ipv6_hdr->src)) &&
                    (!ipv6_addr_is_link_local(&ipv6_hdr->dst)) &&
                    (!ipv6_addr_equal(&addrs[i], &(ipv6_hdr->src)))) {
					thread_wakeup(led_pid); // Wake up the LED thread, indicating a routing message has been received
                    char addr_str[IPV6_ADDR_MAX_STR_LEN];
                    printf("IPv6 ROUTER: forward from src = %s ",
                           ipv6_addr_to_str(addr_str, &(ipv6_hdr->src),
                                            sizeof(addr_str)) );
                    printf("to dst = %s\n",
                           ipv6_addr_to_str(addr_str, &(ipv6_hdr->dst),
                                            sizeof(addr_str)));
                }
            }
        }
        gnrc_pktbuf_release(pkt);
    }
    /* never reached */
    return NULL;
}
#endif

int main(void)
{
    msg_init_queue(main_msg_q, Q_SZ);

    gpio_init(GPIO4, GPIO_OUT);

    led_pid = thread_create(_ledstack, sizeof(_ledstack), (THREAD_PRIORITY_MAIN - 5), THREAD_CREATE_STACKTEST, _led_fwd_eventloop, NULL, "led_fwd");

    udp_pid = thread_create(_udp_receive_stack, sizeof(_udp_receive_stack), (THREAD_PRIORITY_MAIN - 6), THREAD_CREATE_STACKTEST, _udp_receive_loop, NULL, "udp_receive");

    start_server("88");

#ifdef MODULE_GNRC_SIXLOWPAN
    thread_create(_stack, sizeof(_stack), (THREAD_PRIORITY_MAIN - 4), THREAD_CREATE_STACKTEST, _ipv6_fwd_eventloop, NULL, "ipv6_fwd");
#endif
    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
