/*
 * uart.h
 *
 *  Created on: Jul 5, 2019
 *      Author: root
 */

#ifndef UART_H_
#define UART_H_


#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library
#include<string.h>
#include <vector>
#include<string.h>
using namespace std;


#define UART01 "/dev/ttyO1"
#define UART02 "/dev/ttyO2"
#define UART03 "/dev/ttyO3"
#define UART04 "/dev/ttyO4"
#define UART05 "/dev/ttyO5"

class Uart
{

public:

//----------------------Api is used to initate the uart port----------------------

	int file, count;
	 unsigned char receivebuff[0];
	 int Init(char port[],int baudrate)
	   {
		  if ((file = open(port, O_RDWR | O_NOCTTY | O_NDELAY))<0){
		  perror("UART: Failed to open the file.\n");
		  return -1;
	   }
	   struct termios options;               //The termios structure is vital
	   tcgetattr(file, &options);            //Sets the parameters associated with file

	   // Set up the communications options:
	   //   9600 baud, 8-bit, enable receiver, no modem control lines
	   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	   options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
	   tcflush(file, TCIFLUSH);             //discard file information not transmitted
	   tcsetattr(file, TCSANOW, &options);  //changes occur immmediately
	   return 1;
	   }


	 //---------Api is used to send data-----------------------------

	 int send(char p[])
	 {
		// printf("%d",strlen(p));
		if (write(file,p,strlen(p))<0){        //send the string
			   perror("Failed to write to the output\n");
			   return -0;
			}
	 return 1;
	 }



	 //-----------Api is used to recieve data-----------------------------



	int recieve(unsigned char *d)
	  {

		 if ((count=read(file,receivebuff,2))>0)
		 {
			 *d=receivebuff[0];
			 //return count;
		   }
		 return count;

	 }





 };

#endif /* UART_H_ */
