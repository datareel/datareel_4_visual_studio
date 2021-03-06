// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: server.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 10/28/2002
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

Simple stream server demo using the gxSocket class. This server 
example demonstrates how to use a socket Internet address data 
structure to initialize a gxSocket object for servers with multiple 
Ethernet cards.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include <stdlib.h>
#include "gxsocket.h"

int main(int argc, char **argv)
{
  if(argc != 3) {
    cerr << "Usage: " << argv[0] << " ethernet_card_ip port" << "\n" << flush;
    return 1;
  }

  char *ethernet_card_ip = argv[1];
  unsigned short port = (unsigned short) atoi(argv[2]);

  // Initialize the socket Internet address data structure
  gxSocket sin_init;
  gxsSocketAddress sin;
  if(sin_init.InitSocketLibrary() != 0) {
    cout << sin_init.SocketExceptionMessage() << "\n" << flush;
  }
  if(sin_init.InitSIN(&sin, AF_INET, port, ethernet_card_ip) < 0) {
    cout << sin_init.SocketExceptionMessage() << "\n" << flush;
  }

  cout << "Constructing a stream server" << "\n" << flush;
  gxSocket server(SOCK_STREAM, &sin);
  if(!server) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
  
  cout << "Opening stream server on host " << ethernet_card_ip 
       << "\n" << flush;
  cout << "Listening on port " << port << "\n" << flush;

  // Bind the name to the socket
  int rv = server.Bind();
  if(rv < 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
 
  // Listen for connections
  rv = server.Listen();
  if(rv < 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }

  // Accept a connection and block execution until data is
  // received from a client connection to this port.
  rv = server.Accept();
  if(rv < 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }

  // Get the client info (statistical info only)
  char client_name[gxsMAX_NAME_LEN]; int r_port = -1;
  server.GetClientInfo(client_name, r_port);
  cout << "Receiving data from " << client_name 
       << " remote port " << r_port << "\n" << flush;

  const int buf_len = 255; // Receive buffer size
  char sbuf[buf_len];
  // Block until the client sends some data
  rv = server.RawRemoteRead(sbuf, buf_len);
  if(rv < 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
  
  cout << "Received " << rv << " bytes" << "\n" << flush;

  // Print the buffer contents to the stdout for visual conformation
  sbuf[rv] = 0; // Null terminate the buffer
  cout << sbuf;

  cout << "Exiting..." << "\n" << flush;
  server.Close(); // Close the socket connection
  server.ReleaseSocketLibrary();
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
