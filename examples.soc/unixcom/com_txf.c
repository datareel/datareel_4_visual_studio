/************************************/
/*********** Start of File **********/
/************************************/
/* ----------------------------------------------------------- */
/* C Source Code File Name: com_txf.c */
/* C Compiler Used: GCC, HPUX aCC, SOLARIS CC */
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

A simple UNIX program used to transmit a file to a serial
device.
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

int SetBaudRate(int baud_rate);

int main(int argc, char **argv)
{
  int fd, res, baud_rate, rv, binary_mode;
  FILE *fp;
  struct termios newtio;
  unsigned char in_c;  
  char sbuf[1];
    
  if(argc < 3) {
    printf("Serial port transmit program\n");
    printf("Usage 1: %s device name baud rate file name\n", argv[0]);
    printf("Usage 2: %s device baud file \"binary\"\n", argv[0]);
    return 1;
  }

  fd = open(argv[1], O_WRONLY | O_NOCTTY | O_NDELAY);

  if(fd < 0){
    printf("Cannot write to the specified device!\n");
    printf("Exiting...\n");
    return 1;
  }
  
  if(argc > 2 && atoi(argv[2])) baud_rate = SetBaudRate(atoi(argv[2]));
  else baud_rate = SetBaudRate(9600);

  if(argc > 4) {
    rv = strcmp(argv[4], "binary");
    if(rv == 0) binary_mode = 1; else binary_mode = 0;
  }

  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = baud_rate | CS8 | CREAD;
  newtio.c_iflag = IGNBRK;

  if(binary_mode ==  1) {
    newtio.c_oflag = 0;
  }
  else {
    /* Postprocess the output: */
    /* Map chars to upper case and map NL (linefeed) to CR-NL on output */
    newtio.c_oflag = OPOST | OLCUC | ONLCR;    
  }
  
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 1;


#if defined (FASYNC) /* ASYNC I/O is a BDS feature */
  /* Make the file descriptor asynchronous */
  fcntl(fd, F_SETFL, FASYNC);
#endif

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);  

  fp = fopen(argv[3], "r+b");
  if(fp == 0) {
    printf("Could not open the %s file\n", argv[3]);
    printf("Exiting...\n");
    return 1;
  }

  printf("Sending file...\n");
  
  while(feof(fp) == 0) {
    in_c = fgetc(fp);
    sbuf[0] = in_c;
    sbuf[1] = 0;
    res = write(fd, sbuf, 1);  
    if(res < 0) {
      printf("Error writing to %s\n", argv[1]);
      exit(1);
    }
  }

  printf("%s was sent with no errors reported\n", argv[3]); 
  printf("Exiting...\n");
  fclose(fp);
  close(fd);
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

