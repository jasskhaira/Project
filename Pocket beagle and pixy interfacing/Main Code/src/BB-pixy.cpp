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
Uart Bluetooth;
Pixy2BBB pixy;


int x;
int y;
int sig;
int x_min=70;
int x_max=200;
unsigned int maxArea=8000;
unsigned int minArea=1000;
unsigned int width;
unsigned int height;
unsigned int area;
unsigned int newarea;
int i=0;
//uint16_t blocks;
int mySig,Sig_Status=0,Mode_Status=0;
int p;
unsigned char bt_data,Mode='n',Color='n',Manual_inst,j,Track_Mode=1,Find=1;



int Track(char tsig)
{

	   pixy.ccc.getBlocks();  //receive data from pixy

		   if(pixy.ccc.numBlocks)
		  {
			   usleep(10000);
			   if(Find==1)
			   {
				   lpc_link.send("S");
				   Find=0;
			   }
			  sig = pixy.ccc.blocks[i].m_signature;    //get object's signature
			  x = pixy.ccc.blocks[i].m_x;                    //get x position
			  y = pixy.ccc.blocks[i].m_y;                    //get y position
			  width = pixy.ccc.blocks[i].m_width;            //get width
			  height = pixy.ccc.blocks[i].m_height;          //get height
			//  printf("sig = %d    x= %d y= %d  width = %d height= %d \n  area=%d",sig,x,y,width,height,width*height);



			  if(sig==tsig)
			  {
					newarea= width * height;

					printf("newarea %d\n",newarea);

					if(x<x_min )
					{
						lpc_link.send("L");
						printf("Left\n");
						usleep(100000);

					}
/*
					if(x<x_min && newarea>3000)
					{
					  lpc_link.send("l");
					 printf("Left slow\n");
					  usleep(20000);
					}
*/
				   if(x>x_max )
					{
						lpc_link.send("R");
						printf("Right\n");
						usleep(100000);
					}


/*				   if(x>x_max && newarea>3000)
					{
						lpc_link.send("r");
						printf("Right slow\n");
						usleep(20000);
					}
*/
				   if(newarea<7000)
					{
						lpc_link.send("M");
						printf("move\n");
						usleep(100000);
					}
				   else if (newarea>maxArea)
					{
						lpc_link.send("B");
						//printf("back\n");
						usleep(100000);
					}
				 else
				{
					lpc_link.send("S");
					printf("stop\n");
					usleep(300000);
				}
	 	  }}
	  else
	  {
		  usleep(300000);
		  lpc_link.send("F");
		  usleep(300000);
		  Find=1;
	  }

	return 0;
}


int main()
{
	pixy.init();

	pixy.setLamp(0,0);

	lpc_link.Init(UART04,115200);
	Bluetooth.Init(UART01,9600);


begining:
	pixy.setLamp(1,0);
	Mode_Status=0;
	Sig_Status=0;
	lpc_link.send("S");
	Bluetooth.send("Welcome\n");
	usleep(900000);
	Bluetooth.send("Please Select the mode \n");
	usleep(900000);
	Bluetooth.send("A for automatic and M for manual \n");
	usleep(100000);
	pixy.setLamp(0,0);

	while(Mode_Status==0)
	{
		while(Bluetooth.recieve(&Mode)<2);

		//p=Bluetooth.recieve(&Mode);
		//printf("d%\n",p);

		if(Mode=='A')
		{
			Bluetooth.send("Please Select the color to track \n");
			usleep(900000);
			Bluetooth.send("Options- P for Purple Ball \n G for Green Ball \n");
			usleep(900000);

			while(Sig_Status==0)
			{

				usleep(1000);
				while(Bluetooth.recieve(&Color)<2);
				printf("%d\n",p);
				pixy.setLamp(0,0);
				if(Color=='P')
				{
					mySig=1;
					Sig_Status=1;
					Mode_Status=1;
				}

				else if(Color=='Q')
				{
					goto begining;
				}
				else
				{
					Bluetooth.send("Enter a valid Option\n");
					//Sig_Status=0;
				}
			}

		}

		if(Mode=='M')
		{
			Bluetooth.send("Manual Mode\n");
			while(1)
			{
				Bluetooth.recieve(&Manual_inst);
				if(Manual_inst=='M')
				{
					lpc_link.send("M");
					usleep(400000);
					//printf("ffff");
				}
				else if(Manual_inst=='B')
					{
					lpc_link.send("B");
					usleep(400000);
							}
				else if(Manual_inst=='L')
				{
					lpc_link.send("L");
					usleep(400000);
				}
				else if(Manual_inst=='R')
				{
					lpc_link.send("R");
					usleep(400000);

				}
				else if(Manual_inst=='S')
				{
					lpc_link.send("S");
					usleep(400000);
				}
				else if(Manual_inst=='F')
				{
					lpc_link.send("F");
					usleep(400000);
								}
				else if(Manual_inst=='Q')
				{
					goto begining;
				}
				usleep(200);
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
	//pixy.setLamp(1,0);
	while(1)
	{

		Bluetooth.recieve(&j);

		Track(mySig);
		if(j=='Q')
		{
			lpc_link.send("S");
			usleep(30000);
			goto begining;
		}

	}


return 0;
}
