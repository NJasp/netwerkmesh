#include <stdio.h>
// TODO: fix this include not compile
#include <sixlowpan.h>
#include <mrf24j40.h>
#include <mrf24j40_params.h>
#include <mrf24j40_internal.h>
//include <xtimer.h>

uint8_t data = 8;

int main(void)
{
	// setup mrf24j40, possibly not needed
	mrf24j40_t dev;
    mrf24j40_setup(&dev, mrf24j40_params);  
    mrf24j40_reset(&dev);

    return 0;
}
