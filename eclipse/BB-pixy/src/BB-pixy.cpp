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
int maxArea=10000;
int minArea=624;
unsigned int width;
unsigned int height;
unsigned int area;
unsigned int newarea;
int i=0;
//uint16_t blocks;
int mySig,Sig_Status=0,Mode_Status=0;
int p;
unsigned char bt_data,Mode='n',Color='n',Manual_inst,j,Track_Mode=1,Find;



int Track(char tsig)
{

	   pixy.ccc.getBlocks();  //receive data from pixy

		   if(pixy.ccc.numBlocks)
		  {
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
			  printf("sig = %d    x= %d y= %d  width = %d height= %d \n  area=%d",sig,x,y,width,height,width*height);



			  if(sig==tsig)
			  {
					newarea= width * height;

					if(x<x_min)
					{
						lpc_link.send("L");
						//printf("Left\n");
						usleep(300000);

					}
					if(x>x_max)
					{
						lpc_link.send("R");
						//printf("Right\n");
						usleep(300000);
					}
					else if(newarea>minArea)
					{
						lpc_link.send("M");
						//printf("move\n");
						usleep(300000);
					}
					else if (newarea>maxArea)
					{
						lpc_link.send("B");
						//printf("back\n");
						usleep(300000);
					}

				 else
				{
					lpc_link.send("S");
				//	printf("stop\n");
					usleep(300000);
				}


	 	  }}
	  else
	  {
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
	Mode_Status=0;
	Sig_Status=0;
	Bluetooth.send("Welcome\n");
	usleep(900000);
	Bluetooth.send("Please Select the mode \n");
	usleep(900000);
	Bluetooth.send("A for automatic and M for manual \n");
	usleep(100000);


	while(Mode_Status==0)
	{
		while((p=Bluetooth.recieve(&Mode))<2);

		//p=Bluetooth.recieve(&Mode);
		//printf("d%\n",p);

		if(Mode=='A')
		{
			Bluetooth.send("Please Select the color to track \n");
			usleep(900000);
			Bluetooth.send("Options- R for Red \n O for Orange\n");
			usleep(900000);

			while(Sig_Status==0)
			{

				usleep(1000);
				while(Bluetooth.recieve(&Color)<2);
				printf("%d\n",p);
				pixy.setLamp(0,0);
				if(Color=='R')
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
					Bluetooth.send("Enter a valid argument\n");
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
					usleep(300000);
					//printf("ffff");
				}
				else if(Manual_inst=='B')
					{
					lpc_link.send("B");
					usleep(300000);
							}
				else if(Manual_inst=='L')
				{
					lpc_link.send("L");
					usleep(300000);
				}
				else if(Manual_inst=='R')
				{
					lpc_link.send("R");
					usleep(300000);

				}
				else if(Manual_inst=='S')
				{
					lpc_link.send("S");
					usleep(300000);
				}
				else if(Manual_inst=='F')
				{
					lpc_link.send("F");
					usleep(300000);
								}
				else if(Manual_inst=='Q')
				{
					goto begining;
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
	//pixy.setLamp(1,0);
	while(1)
	{

		Bluetooth.recieve(&j);

		Track(mySig);
		if(j=='Q')
		{
			lpc_link.send("S");
			goto begining;
		}

	}


return 0;
}
