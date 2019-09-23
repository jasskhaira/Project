
#include<stdio.h>
#include"uart.h"   // using the termios.h library

Uart Bluetooth;


int main()
{

	Bluetooth.Init(UART04,115200);
	//Bluetooth.send("HELLO");
	unsigned char data;


while(1){
		Bluetooth.send("L");
		printf("L\n");
		usleep(600000);
		Bluetooth.send("R");
		printf("R\n");
		usleep(600000);
		Bluetooth.send("B");
		printf("B\n");
		usleep(1000000);
		Bluetooth.send("M");
		printf("M\n");
		usleep(1000000);
		Bluetooth.send("S");
		printf("S\n");
		usleep(1000000);
		//Bluetooth.send("L");
		//usleep(300000);
		//usleep(10000000);
		printf("sending\n");
}
}

