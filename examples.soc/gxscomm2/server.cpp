// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: server.cpp
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

The program is used to test gxSerialComm class receive
functions by reading characters from a serial port.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <stdlib.h>
#include <string.h>
#include "gxscomm.h"
#include "dfileb.h"

#if defined (__WIN32__) || defined (__DOS__)
#include "r_win32.cpp" // Source code for advanced example
#elif defined (__UNIX__)
#include "r_unix.cpp"  // Source code for advanced example
#else
#error You must define a target platform: __DOS__ __WIN32__ or __UNIX__
#endif

int main(int argc, char **argv)
{
  if(argc < 2) {
    cout << "Serial port receive program" << "\n" << flush;
    cout << "Usage 1: " << argv[0] << " device name [baud rate]" << "\n" 
	 << flush;
    cout << "Usage 2: " << argv[0] << " device name baud rate capture file"
	 << "\n" << flush;
    return 1;
  }

  gxSerialComm dev;
  
  int status = dev.OpenSerialPort(argv[1]);
  if( status < 0) {
    cout << "Cannot open the specified device!" << "\n" << flush;
    cout << "Exiting..." << "\n" << flush;
    return 1;
  }
  else {
    if(status == gxSerialComm::scommREAD_WRITE)
      cout << argv[1] << " open for read/write access." << "\n" << flush;
    else if(status == gxSerialComm::scommREAD_ONLY)
      cout << argv[1] << " open for read only access." << "\n" << flush;
    else if(status == gxSerialComm::scommWRITE_ONLY)
      cout << argv[1] << " open for write only access." << "\n" << flush;
    else {
      cout << "Invalid status opening " << argv[1] << "\n" << flush;
      cout << "Exiting..." << "\n" << flush;
      return 1;
    }
  }
  
  if(argc > 2 && atoi(argv[2])) dev.SetBaudRate(atoi(argv[2]));
  else dev.SetBaudRate(9600);
  
  // dev.SetFlowControl(gxSerialComm::scommNO_FLOW_CONTROL);
  // dev.SetFlowControl(gxSerialComm::scommHARD_FLOW);
  dev.SetFlowControl(gxSerialComm::scommSOFT_FLOW);
  // dev.SetFlowControl(gxSerialComm::scommXON_XOFF);
  
  dev.SetCharacterSize(8);
  dev.SetParity('N');
  dev.SetStopBits(1);
  dev.BinaryMode();
  
  if(dev.InitSerialPort() < 0) {
    cout << "Cannot initialize the specified device!" << "\n" << flush;
    return 1;
  }
      
  if(status == gxSerialComm::scommWRITE_ONLY) {
    cout << "\n" << flush;
    cout << argv[1] << " is open for write only access." << "\n" << flush;
    cout << "You do not have permission to read from this device!" << "\n" 
	 << flush;
    cout << "Exiting..." << "\n" << flush;
    cout << "\n" << flush;
    return 1;
  }

  if(argc > 3)
    SerialRead(&dev, argv[3]); // Write received characters to specified file
  else 
    SerialRead(&dev);

  dev.Close();
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
