
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library
#include"Pixy2BBB.h"


Pixy2BBB pixy;

int main()
{
   int BT,LPC,x,w,h,a,signature,i=0;

   pixy.init();



   if ((BT = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   if ((LPC = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }


   struct termios options;               //The termios structure is vital
   tcgetattr(BT, &options);            //Sets the parameters associated with file

   // Set up the communications options:
   //   9600 baud, 8-bit, enable receiver, no modem control lines
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
   tcflush(BT, TCIFLUSH);             //discard file information not transmitted
   tcsetattr(BT, TCSANOW, &options);  //changes occur immmediately


   struct termios options1;               //The termios structure is vital
   tcgetattr(LPC, &options1);            //Sets the parameters associated with file

   // Set up the communications options:
   //   9600 baud, 8-bit, enable receiver, no modem control lines
   options1.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options1.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
   tcflush(LPC, TCIFLUSH);             //discard file information not transmitted
   tcsetattr(LPC, TCSANOW, &options1);  //changes occur immmediately


   while(1)
   {

	   pixy.ccc.getBlocks();
	   if(pixy.ccc.numBlocks)
	   {
		   signature = pixy.ccc.blocks[i].m_signature;
		   x = pixy.ccc.blocks[i].m_x;
		   w = pixy.ccc.blocks[i].m_width;
		   h = pixy.ccc.blocks[i].m_height;
		   a=w*h;

			  if(signature==1)
			  {
				  if(x<80)
				  {
					  write(LPC,"l",2);
					  usleep(300000);
					  printf("left\n");
				  }
				  if(x>200)
				  {
				 	  write(LPC,"r",2);
				 	  usleep(300000);
				 	 printf("right\n");
				   }
				  if(a<6000)
				  {
					  write(LPC,"g",2);
					  usleep(300000);
					  printf("go\n");
				  }
				  if(a>8000)
				  {
					  write(LPC,"b",2);
					  usleep(300000);
					  printf("back\n");
				  }
			  }}
			  else
			  {
				  write(LPC,"s",2);
				  usleep(300000);
			  }


}}
