#include <stdio.h>

#include <mrf24j40.h>
#include <mrf24j40_internal.h>
#include <mrf24j40_params.h>

#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"

int main(void)
{

    puts("RIOT network stack example application");

    /* get interfaces and print their addresses */
    gnrc_netif_t *netif = NULL;
    while ((netif = gnrc_netif_iter(netif)))
    {
        ipv6_addr_t ipv6_addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
        int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs,
                                  sizeof(ipv6_addrs));

        if (res < 0)
        {
            continue;
        }
        for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++)
        {
            char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

            ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
            printf("My address is %s\n", ipv6_addr);
        }
    }

    /* main thread exits */
    return 0;
}
