#include <stdio.h>
#include <mrf24j40.h>
#include <mrf24j40_internal.h>
#include <mrf24j40_params.h>

int main(void)
{
	mrf24j40_t dev;
    mrf24j40_setup(&dev, mrf24j40_params);  
    mrf24j40_reset(&dev);

    while(1) {
		uint8_t buf;
		mrf24j40_rx_fifo_read(&dev, 0, &buf, 1);

		if (buf != 0) {
			printf("\rreceived: %d:\t'%c'\n", buf, buf);
		}
	}

    return 0;
}
