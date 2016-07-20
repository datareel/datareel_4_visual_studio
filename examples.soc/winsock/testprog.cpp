// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: client.cpp
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

Demo of the gxSocket WinSock functions. This test program is used
to initialize and release various winsock versions and display the
contents of the WSAData structure:

struct WSAData {  
    WORD            wVersion; 
    WORD            wHighVersion; 
    char            szDescription[WSADESCRIPTION_LEN+1]; 
    char            szSystemStatus[WSASYSSTATUS_LEN+1]; 
    unsigned short  iMaxSockets; 
    unsigned short  iMaxUdpDg; 
    char FAR *      lpVendorInfo; // Vendor-specific data structure
}; 

The gxSocket class can be use to automatically or manually
initialize the winsock DLL prior to the use of the Windows 
networking subsystem. Automatic winsock initialization will 
default to winsock version set in the gxSocket constructor.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxsocket.h"
#include "wserror.h"

void PausePrg()
{
  cout << "\n";
  cout << "Press Enter to continue..." << "\n";
  cin.get();
}

void PrintWSAInfo(gxSocket *s)
{
  cout << "WinSock Version Requested: " 
       << (int)LOBYTE(s->socket_data.wVersion) 
       << "." << (int)HIBYTE(s->socket_data.wVersion) << "\n";
  cout << "Current WinSock Version: " 
       << (int)LOBYTE(s->socket_data.wHighVersion)
       << "." << (int)HIBYTE(s->socket_data.wHighVersion) << "\n";
  cout << "Description: " << s->socket_data.szDescription << "\n";
  cout << "System Status: " << s->socket_data.szSystemStatus << "\n";
  cout << "Max number of sockets: " << s->socket_data.iMaxSockets 
       << "\n";
  cout << "MAX UDP datagram size: " << s->socket_data.iMaxUdpDg
       << "\n";
}

void TestWSAVersion(gxSocket *s, gxsSocketLibraryVersion ver)
{
  s->socket_version = ver;
  if(s->InitSocketLibrary() == 0) {
    PrintWSAInfo(s);
    cout << "\n";
    cout << "Simulating a WinSock error and printing the extended error info"
	 << "\n";
    WinSockSetException(WSAEINTR);
    cout << WinSockExceptionMessage() << "\n";
    if(s->ReleaseSocketLibrary() != 0) {
      cout << s->SocketExceptionMessage() << "\n"; 
    }   
  }
  else {
    cout << s->SocketExceptionMessage() << "\n"; 
  } 
}

int main()
{
  gxsSocketLibraryVersion wsa_ver10 = gxSOCKET_WSAVER_ONEZERO;
  gxsSocketLibraryVersion wsa_ver11 = gxSOCKET_WSAVER_ONEONE;
  gxsSocketLibraryVersion wsa_ver20 = gxSOCKET_WSAVER_TWOZERO;
  gxsSocketLibraryVersion wsa_ver22 = gxSOCKET_WSAVER_TWOTWO;

  gxSocket s;

  cout << "Testing winsock version 1.0" << "\n";
  TestWSAVersion(&s, wsa_ver10);
  PausePrg();

  cout << "Testing winsock version 1.1" << "\n";
  TestWSAVersion(&s, wsa_ver11);
  PausePrg();

  cout << "Testing winsock version 2.0" << "\n";
  TestWSAVersion(&s, wsa_ver20);
  PausePrg();

  cout << "Testing winsock version 2.2" << "\n";
  TestWSAVersion(&s, wsa_ver22);

  cout << "\n";
  cout << "Exiting..." << "\n";
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
