#include "spiutil.h"

#define SPI_TIMEOUT		15000

//#define SPI_READ_RX(spi)   		   ((spi)->RX[ssdev])
//#define SPI_WRITE_TX(spi, u32TxData)   ((spi)->TX[ssdev] = (u32TxData))

static int ssdev = 0;

void spi_init(void)
{
}

int spi_read_packet(SPI_T *spi, uint8_t *data, int len)
{
	int timeout, i;
	
	for(i=0; len != 0; i++, len--) {
		timeout = SPI_TIMEOUT;
		while(SPI_IS_BUSY(spi)) {
			if(!timeout--) {
				return(-1);
			}
		}
		SPI_WRITE_TX0(spi, 0);
		SPI_TRIGGER(spi);
		timeout = SPI_TIMEOUT;
		while(SPI_IS_BUSY(spi)) {
			if(!timeout--) {
				return(-1);
			}
		}
		data[i] = SPI_READ_RX0(spi);
	}
	return(0);
}

int spi_write_packet(SPI_T *spi, uint8_t *data, int len)
{
	int timeout, i;

	for(i=0; len != 0; i++, len--) {
		timeout = SPI_TIMEOUT;
		while(SPI_IS_BUSY(spi)) {
			if(!timeout--) {
				return(-1);
			}
		}
		SPI_WRITE_TX0(spi, data[i]);
		SPI_TRIGGER(spi);
	}
	return(0);
}

void spi_select_device(SPI_T *spi, int ss)
{
	ssdev = ss;
	if(ss == 0) {
		SPI_SET_SS0_LOW(spi);
	}
	else {
		SPI_SET_SS1_LOW(spi);
	}
}

void spi_deselect_device(SPI_T *spi, int ss)
{
	ssdev = ss;
	if(ss == 0) {
		SPI_SET_SS0_HIGH(spi);
	}
	else {
		SPI_SET_SS1_HIGH(spi);
	}
}
