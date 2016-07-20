/************************************/
/*********** Start of File **********/
/************************************/
/* ----------------------------------------------------------- */
/* C Source Code File Name: com_rx.c */
/* Produced By: DataReel Software Development Team */
/* File Creation Date: 12/16/1999 */
/* Date Last Modified: 07/09/2007 */
/* Copyright (c) 2001-2007 DataReel Software Development */
/* ----------------------------------------------------------- */
/* ------------- Program Description and Details ------------- */
/* ----------------------------------------------------------- */
/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
USA

A simple UNIX program used to read characters from a serial port
and print them to the console.
*/
/* ----------------------------------------------------------- */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define rx_buf_size 255

int SetBaudRate(int baud_rate);
volatile int STOP=FALSE;

int main(int argc, char **argv)
{
  int fd, res;
  struct termios newtio;
  char buf[rx_buf_size];
  int baud_rate;
    
  if(argc < 2) {
    printf("Serial port receive program\n");
    printf("Usage: %s device name [baud rate]\n", argv[0]);
    return 1;
  }
  
  /* Open the serial port without waiting for the requested modem connection */
  fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd <0) {perror(argv[1]); exit(1); }

  if(argc > 2 && atoi(argv[2])) baud_rate = SetBaudRate(atoi(argv[2]));
  else baud_rate = SetBaudRate(9600);

  bzero(&newtio, sizeof(newtio));

  /* Ignore the modem control lines with the CLOCAL flag */
  newtio.c_cflag = baud_rate | CS8 | CREAD | CLOCAL; 
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;

  /* Set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;
      
  newtio.c_cc[VTIME]    = 0;   /* Inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;   /* Blocking read until 1 char is received */

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  printf("%s open for read/write access\n", argv[1]);
  printf("Wating to receive...");
  printf("Press Ctrl C to exit.\n");
  printf("\n");

  res = 0;
  while(STOP==FALSE) {
    usleep(100000); /* Allow for I/O recovery time */
    res = read(fd,buf,255);   /* Returns after 1 char has been input */
    buf[res]=0;               /* Ensure null termination */
    if(res > 0) {
      printf("%s", buf);
      fflush(stdout);
    }
  }

  printf("Exiting...\n");
  return 0;
}

int SetBaudRate(int baud_rate)
/* Set the baud rate. Defaults to 9600 */
{
  int br;
  switch(baud_rate) {
    case 0: // 0 baud (drop DTR)
      br = B0;
      break;
    case 50: 
      br = B50;
      break;
    case 75: 
      br = B75;
      break;
    case 110: 
      br = B110;
      break;
    case 134: 
      br = B134;
      break;
    case 150: 
      br = B150;
      break;
    case 200: 
      br = B200;
      break;
    case 300: 
      br = B300;
      break;
    case 600: 
      br = B600;
      break;
    case 1200: 
      br = B1200;
      break;
    case 1800: 
      br = B1800;
      break;
    case 2400: 
      br = B2400;
      break;
    case 4800: 
      br = B4800;
      break;
    case 9600: 
      br = B9600;
      break;
    case 19200: 
      br = B19200;
      break;
    case 57600: 
      br = B57600;
      break;
    case 115200: 
      br = B115200;
      break;
    case 230400: 
      br = B230400;
      break;
    case 460800: 
      br = B460800;
      break;
    default:
      br = B9600;
      break;
  }
  return br;
}
/* ----------------------------------------------------------- */
/************************************/
/************ End of File ***********/
/************************************/
