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

unsigned char bt_data[100];


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

	pixy.init();

	pixy.setLamp(1,1);

	lpc_link.Init(Uart04,115200);

	while(1)
	{

		getpixydata();
		track(1);

	}
return 0;
}
