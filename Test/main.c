#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>
#include <mrf24j40_internal.h>
//include <xtimer.h>

mrf24j40_t sender;
uint8_t data = 8;

int main(void)
{
    mrf24j40_setup(&sender, mrf24j40_params);  
    mrf24j40_reset(&sender);
    //gpio_clear(sender.params.reset_pin);
    //puts("Hallo Welt!\n");

    //printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    //printf("This board features a(n) %s MCU.\n", RIOT_MCU);
    mrf24j40_tx_prepare(&sender);
    printf("\n\r%d", mrf24j40_tx_load(&sender, &data, 1, 0));
    
    while(1)
    {
    mrf24j40_tx_exec(&sender);
    //mrf24j40_tx_normal_fifo_write(&sender,0,&data, 1);
    }

	// test board by setting random shit
	//printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
	//mrf24j40_set_addr_short(dev, 0x3);
	//mrf24j40_set_txpower(dev, 100);
	//printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
    return 0;
}
