#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>

mrf24j40_t sender;
char *data = "xy";

int main(void)
{
    mrf24j40_setup(&sender, mrf24j40_params);
    mrf24j40_reset(&sender);
    
    mrf24j40_tx_prepare(&sender);

    while (1)
    {
        mrf24j40_tx_load(&sender, (uint8_t *)data, 2, 0);
        mrf24j40_tx_exec(&sender);
    }

    return 0;
}
