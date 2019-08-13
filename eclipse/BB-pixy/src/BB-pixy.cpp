//============================================================================
// Name        : BB-pixy.cpp
// Author      : Jaspreet Singh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include"Pixy2BBB.h"
#include"uart.h"
#include"TPixy2.h"
using namespace std;

Uart lpc_link;
Uart Bt_link;
Pixy2BBB pixy;


int x;
int y;
int sig;
int x_min=70;
int x_max=200;
int maxArea=624;
int minArea=624;
unsigned int width;
unsigned int height;
unsigned int area;
unsigned int newarea;
int i=0;
uint16_t blocks;
int mySig,Sig_Status=0,Mode_Status=0;

unsigned char bt_data,Mode,Color;



int getpixydata()
{

	  blocks = pixy.ccc.getBlocks();  //receive data from pixy
	  sig = pixy.ccc.blocks[i].m_signature;    //get object's signature
	  x = pixy.ccc.blocks[i].m_x;                    //get x position
	  y = pixy.ccc.blocks[i].m_y;                    //get y position
	  width = pixy.ccc.blocks[i].m_width;            //get width
	  height = pixy.ccc.blocks[i].m_height;          //get height
	  printf("sig = %d    x= %d y= %d  width = %d height= %d \n  area=%d",sig,x,y,width,height,width*height);

	return 0;
}





int track(int tsig)
{
	if(sig==tsig){
		newarea= width * height;

		if(x<x_min)
		{
			lpc_link.send("L");
			printf("Left\n");

		}
		if(x>x_max)
		{
			lpc_link.send("R");
			printf("Right\n");
		}
		else if(newarea<minArea)
		{
			lpc_link.send("M");
			printf("move\n");
		}
		else if (newarea>maxArea)
		{
			lpc_link.send("B");
			printf("back\n");
		}

   else
	{
		lpc_link.send("S");
		printf("stop\n");
	}
}
	return 0;


}


int main()
{

	Uart lpc_link;
	Uart Bluetooth;
	pixy.init();

	pixy.setLamp(1,1);

	lpc_link.Init(UART04,115200);
	Bluetooth.Init(UART01,9600);


begining:
	Mode_Status=0;
	Sig_Status=0;
	Bluetooth.send("Welcome\n");
	usleep(10000);
	Bluetooth.send("Please Select the mode \n");
	usleep(10000);
	Bluetooth.send("A for automatic and M for manual \n");
	usleep(10000);
	pixy.setLamp(0,0);
	while(Mode_Status==0)
	{
		while(Bluetooth.recieve(&Mode)!=1);
		if(Mode=='A')
		{
			Bluetooth.send("Please Select the color to track \n");
			usleep(10000);
			Bluetooth.send("Options- R for Red \n o for Orange\n");
			usleep(10000);

			while(Sig_Status==0)
			{
				while(Bluetooth.recieve(&Color)!=1);
				if(Color=='R')
				{
					mySig=1;
					Sig_Status=1;
				}

				else if(Color=='Q')
				{
					goto begining;
				}
				else
				{
					Bluetooth.send("Enter a valid argument\n");
					//Sig_Status=0;
				}
			}

		}


		else if(Mode=='Q')
			{
				goto begining;
			}

		else
			{
				Bluetooth.send("Please Choose an valid option \n");
			}


	}


	Bluetooth.send("Tracking \n");
	while(Mode=='A')
	{
		Bluetooth.recieve(&Mode);
		getpixydata();
		track(mySig);
		if(Mode=='Q')
		{
			goto begining;
		}
	}


return 0;
}
