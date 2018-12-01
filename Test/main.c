#define MQ_S 32

#include <stdio.h>

#include <mrf24j40.h>
#include <mrf24j40_internal.h>
#include <mrf24j40_params.h>

#include "msg.h"
#include "net/gnrc/netreg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"

void request_ipv6_addresses_from_connected_devices(ipv6_addr_t * address);
void incoming_data(gnrc_pktsnip_t * pkt);

gnrc_netreg_entry_t mrf24j40;
gnrc_pktsnip_t *pkt = NULL;
mrf24j40_t sender;
ipv6_addr_t src_ipv6_addr[GNRC_NETIF_IPV6_ADDRS_NUMOF];
msg_t msg_queue[MQ_S];
msg_t msg_to_send;
msg_t msg_to_receive;
char *data;

int main(void)
{

    mrf24j40_setup(&sender, mrf24j40_params);
    mrf24j40_reset(&sender);

    msg_init_queue(msg_queue, MQ_S);

    gnrc_netreg_entry_init_pid(&mrf24j40, GNRC_NETREG_DEMUX_CTX_ALL, sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_IPV6, &mrf24j40);

    request_ipv6_addresses_from_connected_devices(src_ipv6_addr);

    irq_disable();

    while (1)
    {
        msg_receive(&msg_to_receive);
        switch (msg_to_receive.type)
        {
        case GNRC_NETAPI_MSG_TYPE_RCV:
            pkt = msg_to_receive.content.ptr;
            incoming_data(pkt);
            break;
        case GNRC_NETAPI_MSG_TYPE_SND:
            pkt = msg_to_receive.content.ptr;
            //_handle_outgoing_pkt(pkt);
            break;
        case GNRC_NETAPI_MSG_TYPE_SET:
        case GNRC_NETAPI_MSG_TYPE_GET:
            msg_reply(&msg_to_receive, &msg_to_send);
            break;
        default:
            break;
        }
    }

    return 0;
}

void request_ipv6_addresses_from_connected_devices(ipv6_addr_t * address)
{
    gnrc_netif_t *netif = NULL;
    puts("hoi");

    while ((netif = gnrc_netif_iter(netif)))
    {
        puts("hoi2");
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
}

void incoming_data(gnrc_pktsnip_t * pkt) {
    printf("\rData ontvangen: %s", (char*) pkt->data);
}
