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

This program is used to the gxDatagram class.
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
#include "gxdatagm.h"

int ReadBlock(gxDatagram *server, gxBlockHeader &gx)
{
  cout << "Reading a raw block of data" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length; 

  char *buf = new char[block_size];
  if(server->ReadRemoteBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout.write(buf, block_size);
  cout << server->BytesRead() << " bytes received." << "\n" << flush;
  
  delete[] buf;
  return 0;
}

int AnswerRequest(gxDatagram *server, gxBlockHeader &gx)
{
  cout << "Client has requested a block" << "\n" << flush;
 
  char *test_block = "The quick brown fox jumps over the lazy dog \
0123456789\n";

  __ULWORD__ block_size = (__ULWORD__)gx.block_length; 

  char *buf = new char[block_size];
  if(server->ReadRemoteBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout << "Client has requested block: ";
  cout.write(buf, block_size);
  cout << "\n" << flush;
  delete[] buf;

  cout << "Answering request" << "\n" << flush;
  
  if(server->WriteRemoteBlock((char *)test_block, strlen(test_block))
			      != 0) return 1;
  return 0;
}

int ChangeBlock(gxDatagram *server, gxBlockHeader &gx)
{
  cout << "Received a change block request" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length; 

  char *buf = new char[block_size];
  if(server->ReadRemoteBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout << "Client requested that block \"";
  cout.write(buf, block_size);
  cout << "\" be changed to:" << "\n" << flush;
  delete[] buf;

  gxBlockHeader block_header;
  if(server->ReadClientHeader(block_header) != 0) return 1;

  char *block = new char[(__ULWORD__)block_header.block_length];
  if(server->ReadRemoteBlock((char *)block, block_header) != 0) {
    delete[] block;
    return 1;
  }

  cout.write(block, (__ULWORD__)block_header.block_length);  
  cout.flush();

  delete[] block;
  return 0;
}

int AddBlock(gxDatagram *server, gxBlockHeader &gx)
{
  cout << "Received an add block request" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length; 

  char *block = new char[block_size];
  if(server->ReadRemoteBlock(block, gx) != 0) {
    delete[] block;
    return 1;
  }

  cout << "Client has requested the following block be added:" << "\n" 
       << flush;
  cout.write(block, block_size);
  cout.flush();
  delete[] block;
  
  return 0;
}

int DeleteBlock(gxDatagram *server, gxBlockHeader &gx)
{
  cout << "Received a delete block request" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length; 

  char *buf = new char[block_size];
  if(server->ReadRemoteBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }
  
  cout << "Client has requested block \"";
  cout.write(buf, block_size);
  cout << "\" be deleted" << "\n" << flush;
  delete[] buf;
  return 0;
}

int main(int argc, char **argv)
{
  // Check arguments. Should be only one: the port number to bind to.
  if(argc != 2) {
    cerr << "Usage: " << argv[0] << " port" << "\n" << flush;
    return 1;
  }

  gxDatagram server;
  unsigned short port = (unsigned short) atoi(argv[1]);

  cout << "Initializing the GX datagram server..." << "\n" << flush;
  if(server.DatagramServer(port) != 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
  
  // Get the host name assigned to this machine
  char hostname[gxsMAX_NAME_LEN];
  if(server.HostName(hostname) != 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
  cout << "Opening datagram server on host " << hostname << "\n" << flush;

  // Find out what port was really assigned 
  int assigned_port;
  if(server.PortNumber(assigned_port) != 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }
  cout << "Port assigned is " << assigned_port << "\n" << flush;
    
  while(1) { // Block until the next read. 
    // Read the block following a client connection
    gxBlockHeader gx;
    if(server.ReadClientHeader(gx) != 0) {
      cout << server.SocketExceptionMessage() << "\n" << flush;
      return 1;
    }

    // Get the client info
    char client_name[gxsMAX_NAME_LEN]; int r_port = -1;
    server.GetClientInfo(client_name, r_port);
    cout << client_name << " connecting on port " << r_port << "\n" << flush;

    // Read the status byte to determine what to do with this block
    __ULWORD__ block_status = gx.block_status;
    __SBYTE__ status = (__SBYTE__)((block_status & 0xFF00)>>8);

    switch(status) { 
      // Process each block of data
      case gxAcknowledgeBlock:
	cout << "Received an acknowledge block command" << "\n" << flush;
	cout << "Sending acknowledgment" << "\n" << flush;
	if(server.WriteRemoteAckBlock() != 0)
	  cout << server.SocketExceptionMessage() << "\n" << flush;	  
	break;

      case gxAddRemoteBlock:
	if(AddBlock(&server, gx) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	break;

      case gxChangeRemoteBlock:
	if(ChangeBlock(&server, gx) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	break;

      case gxRequestBlock:
	if(AnswerRequest(&server, gx) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	break;

      case gxDeleteRemoteBlock:
	if(DeleteBlock(&server, gx) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	break;

      case gxSendBlock :  
	if(ReadBlock(&server, gx) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	break;

      case gxCloseConnection : 
	cout << "Client sent a close connection command" << "\n" << flush;
	break;

      case gxKillServer:
	cout << "Client shutdown the server" << "\n" << flush;
	server.Close();
	return 0;
	
      default:
	cout << "Received bad block command from client" << "\n" << flush;
	break;
    }
  }

  cout << "Exiting..." << "\n" << flush;
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
