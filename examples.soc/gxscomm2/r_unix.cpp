// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: r_unix.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/20/1999
// Date Last Modified: 01/01/2009
// Copyright (c) 2001-2009 DataReel Software Development
// ----------------------------------------------------------- // 
// ------------- Program Description and Details ------------- // 
// ----------------------------------------------------------- // 
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

Code used by UNIX systems to read data from a serial port using
a signal handler. 
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#include <fcntl.h>

// #ifndef __USE_SIGNAL_IO__
// #define __USE_SIGNAL_IO__
// #endif 

#ifndef __USE_SIGNAL_IO__ // Do not interrupt driven I/O 
void SerialRead(gxSerialComm *dev, char *dumpfile = 0)
{
  DiskFileB outfile;
  if(dumpfile) {
    cout << "Writing output to the " << dumpfile << " file" << "\n" << flush; 
    outfile.df_Create(dumpfile);
    if(!outfile) {
      cout << "\n" << flush;
      cout << "Cannot open the: " << dumpfile << " capture file"
	   << "\n" << flush;
      cout << "Exiting..." << "\n" << flush;
      cout << "\n" << flush;
      return;
    }
  }

  cout << "Waiting to receive..." << "\n" << flush;
  cout << "Press Ctrl C to terminate." << "\n" << flush;

  const int rxBufferLen = 255;
  char buf[rxBufferLen];

  // Loop while waiting for input
  while(1) {
    usleep(100000); // Allow for I/O recovery time 
    dev->RawRead((char *)buf, rxBufferLen);
    if(dev->BytesRead() > 0) {
      buf[dev->BytesRead()] = 0; // Null terminate the string
      if(dumpfile) { // Capture data to a file
	outfile.df_Write((char *)buf, dev->BytesRead());
	outfile.df_Flush();
      }
      else { // Write the data to the console
	cout << buf << flush;
      }
    }
  }
}

#else // Using interrupt driven I/O
#include <sys/signal.h>

int comSTOP=0;
void signal_handler_IO(int status);  // Signal handler
int wait_flag=1;  // true while no signal received 

void SerialRead(gxSerialComm *dev, char *dumpfile = 0)
{
  DiskFileB outfile;
  if(dumpfile) {
    cout << "Writing output to the " << dumpfile << " file" << "\n" << flush; 
    outfile.df_Create(dumpfile);
    if(!outfile) {
      cout << "\n" << flush;
      cout << "Cannot open the: " << dumpfile << " capture file"
	   << "\n" << flush;
      cout << "Exiting..." << "\n" << flush;
      cout << "\n" << flush;
      return;
    }
  }

  struct sigaction saio; // Signal action
  const int rxBufferLen = 255;
  char buf[rxBufferLen];
    
  // Install the signal handler before making the device asynchronous
  saio.sa_handler = signal_handler_IO;
  saio.sa_flags = 0;
  sigaction(SIGIO, &saio, 0);
  
  // Allow this process to receive SIGIO
#if defined (FASYNC) // ASYNC I/O is a BDS feature
  // A SIGIO signal will be sent whenever input or
  // output becomes possible on this file descriptor.
  fcntl(dev->DeviceHandle(), F_SETOWN, getpid());
  fcntl(dev->DeviceHandle(), F_SETFL, FASYNC);

#elif defined (FIOSSAIOSTAT) // HPUX specific
  // Set the process ID that will receive the SIGIO signals
  ioctl(dev->DeviceHandle(), FIOSSAIOOWN, dev->GetTermIOS());
  ioctl(dev->DeviceHandle(), FIOSSAIOSTAT, dev->GetTermIOS());  

#else
#error Cannot receive SIGIO signals using ASYNC IO
#endif

  cout << "Waiting to receive..." << "\n" << flush;
  cout << "Press Ctrl C to terminate." << "\n" << flush;
  
  // Loop while waiting for input
  while(comSTOP == 0) {
    // After receiving SIGIO check the wait_flag to see if input
    // is available and can be read
    if(wait_flag == 0) {
      if(dev->RawRead((char *)buf, 255) < 0) {
	// Stop loop if an error occurs
	cerr << "\n" << flush;
	cerr << "Read error!" << "\n" << flush;
	cerr << "Exiting..." << "\n" << flush;
	comSTOP = 1; 
      }
      buf[dev->BytesRead()] = 0; // Null terminate the string
      if(dumpfile) { // Capture data to a file
	outfile.df_Write((char *)buf, dev->BytesRead());
	outfile.df_Flush();
      }
      else { // Write the data to the console
	cout << buf << flush;
      }
      wait_flag = 1; // Wait for new input 
    }
  }
}

void signal_handler_IO(int status)
// Signal handler. Sets wait_flag to 0, to indicate the above loop that  
// characters have been received. 
{
  cout.flush();
  wait_flag = 0;
}

#endif // __USE_SIGNAL_IO__
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
