#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>

mrf24j40_t sender;
uint8_t data = 2;

int main(void)
{
    mrf24j40_setup(&sender, mrf24j40_params);
    mrf24j40_reset(&sender);
    
    mrf24j40_tx_prepare(&sender);
    mrf24j40_tx_load(&sender, &data, 1, 0);

    while (1)
    {
        mrf24j40_tx_exec(&sender);
    }

    return 0;
}
