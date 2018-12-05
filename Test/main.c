#define MQ_S 8

#include <stdio.h>

#include <mrf24j40.h>
#include <mrf24j40_internal.h>
#include <mrf24j40_params.h>

#include "msg.h"
#include "thread.h"
#include "net/gnrc/netreg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"

//void request_ipv6_addresses_from_connected_devices(ipv6_addr_t *address);
void incoming_data(gnrc_pktsnip_t *pkt);
void *rcv_thread(void *arg);
char rcv_thread_stack[THREAD_STACKSIZE_MAIN];

gnrc_netreg_entry_t mrf24j40;
gnrc_pktsnip_t *pkt;
mrf24j40_t sender;

uint8_t raw_data[2] = {'A', 'A'};
//uint8_t ly1head[2] = {'C', 'C'};           //The layer 1 header

//ipv6_addr_t src_ipv6_addr[GNRC_NETIF_IPV6_ADDRS_NUMOF];
msg_t msg_queue[MQ_S];

int main(void)
{

    ///request_ipv6_addresses_from_connected_devices(src_ipv6_addr);

    //pkt = gnrc_pktbuf_add(NULL, raw_data, sizeof(raw_data), GNRC_NETTYPE_UNDEF);

    thread_create(rcv_thread_stack, sizeof(rcv_thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, rcv_thread, NULL, "fred_send");

    //Short address is 6946

    return 0;
}

void *rcv_thread(void *arg)
{
    mrf24j40_setup(&sender, mrf24j40_params);
    mrf24j40_reset(&sender);

    msg_init_queue(msg_queue, MQ_S);

    gnrc_netreg_entry_init_pid(&mrf24j40, GNRC_NETREG_DEMUX_CTX_ALL, sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &mrf24j40);

    (void)arg;
    //msg_t m;
    while (1)
    {
        //msg_receive(&m);
        //printf("Got msg from %" PRIkernel_pid "\n", m.sender_pid);
        pkt = gnrc_pktbuf_add(NULL, raw_data, sizeof(raw_data), GNRC_NETTYPE_UNDEF);
        incoming_data(pkt);
    }
    return NULL;
}

/*void request_ipv6_addresses_from_connected_devices(ipv6_addr_t *address)
{
    gnrc_netif_t *netif = NULL;

    while ((netif = gnrc_netif_iter(netif)))
    {
        //Get addresses of all connected devices
        int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, address, sizeof((*address)));

        if (res < 0)
        {
            continue;
        }
        for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++)
        {
            char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

            ipv6_addr_to_str(ipv6_addr, &address[i], IPV6_ADDR_MAX_STR_LEN);
            printf("\rDevice found! Address is:  %s\n", ipv6_addr);
        }
    }
}*/

void incoming_data(gnrc_pktsnip_t *pkt)
{
    if (pkt == NULL)
    {
        puts("Error: unable to copy data to packet buffer\n");
        return;
    }
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UNDEF, 80, pkt))
    {
        puts("Error: no thread is interested");
        gnrc_pktbuf_release(pkt);
        return;
    }
    //printf("\rData ontvangen: %s", (char *)pkt->data);
}
