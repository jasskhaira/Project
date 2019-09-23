#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <inttypes.h>

uint32_t millis();
void delayMicroseconds(uint32_t us);
int spi_open(const char *__file, int __oflag);

class Link2SPI
{
private:
	// Choose SPI device from Beaglebouint32_t argne Black
	const char *spi = "/dev/spidev2.0";
	uint8_t spiMode = 3, wordSize = 8;
	uint32_t spiSpeed = 15000000;
	int file = -1;
public:
	int8_t open()
	{
		if ((file = spi_open(spi, O_RDWR)) < 0) {
			printf("Error opening SPI device.");
			return -1;
		}
		if (ioctl(file, SPI_IOC_WR_MODE, &spiMode) == -1) {
			printf("Error setting SPI mode.");
			return -1;
		}
		if (ioctl(file, SPI_IOC_WR_BITS_PER_WORD, &wordSize) == -1) {
			printf("Error configuring word size.");
			return -1;
		}
		if (ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed) == -1) {
			printf("Error adjusting SPI speed.");
			return -1;
		}
		return 0;
	}

	void close()
	{
		//SPI.endTransaction();
	}

	int16_t recv(uint8_t *buf, uint8_t len, uint16_t *cs=NULL)
	{
		uint8_t i;

		struct spi_ioc_transfer spiXfer;
		memset(&spiXfer, 0, sizeof(spi_ioc_transfer));
		spiXfer.tx_buf = (unsigned long) NULL;
		spiXfer.rx_buf = (unsigned long) buf;
		spiXfer.len = len;
		if (ioctl(file, SPI_IOC_MESSAGE(1), &spiXfer) < 0) {
			printf("SPI transmit error");
			return -1;
		}

		if (cs)
			*cs = 0;
		for (i=0; i<len; i++)
		{
			//buf[i] = SPI.transfer(0x00);
			if (cs)
				*cs += buf[i];
		}

		return len;
	}

	int16_t send(uint8_t *buf, uint8_t len)
	{
		//uint8_t i;
		//for (i=0; i<len; i++)
		//SPI.transfer(buf[i]);

		struct spi_ioc_transfer spiXfer;
		memset(&spiXfer, 0, sizeof(spi_ioc_transfer));
		spiXfer.tx_buf = (unsigned long) buf;
		spiXfer.rx_buf = (unsigned long) NULL;
		spiXfer.len = len;
		if (ioctl(file, SPI_IOC_MESSAGE(1), &spiXfer) < 0) {
			printf("SPI transmit error");
			return -1;
		}

		return len;
	}
};




uint32_t millis() {
	struct timespec spec;

	clock_gettime(CLOCK_MONOTONIC, &spec);

	return (spec.tv_nsec / 1000000);
}

void delayMicroseconds(uint32_t us) {
	usleep(us);
}

int spi_open(const char *file, int oflag) {
	return open(file, oflag);
}


int main()


{
	Link2SPI spi;
	  uint8_t i, lenReceived, recvBuf[32];


	  uint8_t versionRequest[] =
	  {
	    0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
	    0xc1,  // second byte of no_checksum_sync
	    0x0e,  // this is the version request type
	    0x00   // data_length is 0
	  };

	  spi.open();
	  spi.send(versionRequest, 4);
	  delayMicroseconds(1000);
	  lenReceived = spi.recv(recvBuf, 6 + 16);

	  printf("Received %hhu bytes.\n", lenReceived);
	    for (i=0; i<lenReceived; i++)
	      printf("%hhu: 0x%hhx\n", i, recvBuf[i]);



return 1;
}









