#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>

mrf24j40_t sender;
char *data = "xy";

int main(void)
{
    mrf24j40_setup(&sender, mrf24j40_params);  
    mrf24j40_reset(&sender);

    //puts("Hallo Welt!\n");

    //printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    //printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    while(1)
    {

    //mrf24j40_tx_prepare(&sender);
    //mrf24j40_tx_load(&sender, (uint8_t*)data, 2, 0);

    //mrf24j40_tx_exec(&sender);
        //mrf24j40_reg_write_short(&sender, 0xf8f8, )
    }

	// test board by setting random shit
	//printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
	//mrf24j40_set_addr_short(dev, 0x3);
	//mrf24j40_set_txpower(dev, 100);
	//printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
    return 0;
}
