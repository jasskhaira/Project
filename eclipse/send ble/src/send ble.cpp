
#include<stdio.h>
#include"uart.h"   // using the termios.h library

Uart Bluetooth;

int main(){

	Bluetooth.Init(UART01,9600);
	Bluetooth.send("HELLO");
	unsigned char data;
	while(1)
	{
		//printf("sending\n");
		//Bluetooth.send("H");
		//usleep(1000);

		Bluetooth.recieve(&data);

			if (data=='m')
		{
			printf("data%c\n",data);
			//data='k';
		}}
}


