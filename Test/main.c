#define MQ_S 32

#include <stdio.h>

#include <mrf24j40.h>
#include <mrf24j40_internal.h>
#include <mrf24j40_params.h>

#include "msg.h"
#include "net/gnrc/netreg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"

void request_ipv6_addresses_from_connected_devices(void);

gnrc_netreg_entry_t mrf24j40;
mrf24j40_t sender;
msg_t msg_queue[MQ_S];
msg_t msg_to_send;
msg_t msg_to_receive;
char * data;

int main(void)
{

    mrf24j40_setup(&sender, mrf24j40_params);
    mrf24j40_reset(&sender);

    msg_init_queue(msg_queue, MQ_S);

    gnrc_netreg_entry_init_pid(&mrf24j40, GNRC_NETREG_DEMUX_CTX_ALL, sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_IPV6, &mrf24j40);

    request_ipv6_addresses_from_connected_devices();

    irq_disable();

    data = "test123";
    msg_to_send.type = 12;
    msg_to_send.content.ptr = data;

    msg_send_to_self(&msg_to_send);
    //msg_queue_print();

    msg_receive(&msg_to_receive);
    printf("\r%s", (char*)msg_to_receive.content.ptr);

    while (1)
    {
        //printf("\nhoihoi");
    }

    return 0;
}

void request_ipv6_addresses_from_connected_devices(void)
{
    gnrc_netif_t *netif = NULL;

    while ((netif = gnrc_netif_iter(netif)))
    {
        //Get addresses of all connected devices
        ipv6_addr_t ipv6_addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
        int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs, sizeof(ipv6_addrs));

        if (res < 0)
        {
            continue;
        }
        for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++)
        {
            char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

            ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
            printf("\rDevice found! Address is:  %s\n", ipv6_addr);
        }
    }
}
