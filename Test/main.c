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

mrf24j40_t sender;
char *data = "xy";

int main(void)
{
    // senderparams vullen
    

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

    return 0;
}
