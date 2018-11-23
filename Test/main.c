/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>

int main(void)
{
	puts("Hallo Welt!\n");

	printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
	printf("This board features a(n) %s MCU.\n", RIOT_MCU);

	mrf24j40_t *dev = malloc(sizeof(mrf24j40_t));
	mrf24j40_params_t params = MRF24J40_PARAMS;
	mrf24j40_setup(dev, &params);

	// test board by setting random shit
	printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
	mrf24j40_set_addr_short(dev, 0x3);
	mrf24j40_set_txpower(dev, 100);
	printf("mrf addr: %d\nmrf txpower: %d\n", mrf24j40_get_addr_short(dev), mrf24j40_get_txpower(dev));
    return 0;
}
