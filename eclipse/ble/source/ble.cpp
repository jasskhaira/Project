
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library

int main(){
   int file, count;

   if ((file = open("/dev/ttyO0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
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


   unsigned char receive[0];
   unsigned char rec;



   while (1)
   {

        //declare a buffer for receiving data
   if((count=read(file,receive,2))>0)
   {
      printf("The following was read in [%d]: %s\n",count,receive);
      rec=receive[0];
      if(rec=='M')
      {
      printf("data,%c\n",rec);
      }
   }

   }

   close(file);
   return 0;
}
