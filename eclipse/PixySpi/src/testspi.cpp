/*
 * testspi.cpp
 *
 *  Created on: Jun 28, 2019
 *      Author: root
 */

#include"BBSpi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>



int main()
{
	SPI_config conf;

	conf.spi_dev_path=SPIDEV1_PATH;
	conf.spi_mode=SPI_MODE3;
	conf.spi_bus_speedHZ=500000;
	conf.spi_bytes_num=8;
	conf.spi_data_bits_No=8;

	unsigned char send[1]={"66"};
	unsigned char receive[1];


	SpiInit(conf);
	Spitransfer(send,receive,8);
return 1;
}




