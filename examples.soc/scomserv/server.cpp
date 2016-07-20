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

The program is used to test gxSerialCommServer class.
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
#include "scomserv.h"

int ReadBlock(gxSerialCommServer *server, gxBlockHeader &gx)
{
  cout << "Reading a raw block of data" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length;

  char *buf = new char[block_size];
  if(server->ReadBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout.write(buf, block_size);
  cout << server->BytesRead() << " bytes received." << "\n" << flush;
  
  delete[] buf;
  return 0;
}

int AnswerRequest(gxSerialCommServer *server, gxBlockHeader &gx)
{
  cout << "Client has requested a block" << "\n" << flush;
 
  char *test_block = "The quick brown fox jumps over the lazy dog \
0123456789\n";

  __ULWORD__ block_size = (__ULWORD__)gx.block_length;
  char *buf = new char[block_size];
  if(server->ReadBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout << "Client has requested block: ";
  cout.write(buf, block_size);
  cout << "\n" << flush;
  delete[] buf;

  cout << "Answering request" << "\n" << flush;

  if(server->WriteBlock((char *)test_block, strlen(test_block)) != 0)
    return 1;

  return 0;
}

int ChangeBlock(gxSerialCommServer *server, gxBlockHeader &gx)
{
  cout << "Received a change block request" << "\n" << flush;
 
  __ULWORD__ block_size = (__ULWORD__)gx.block_length;
  char *buf = new char[block_size];
  if(server->ReadBlock(buf, gx) != 0) {
    delete[] buf;
    return 1;
  }

  cout << "Client requested that block \"";
  cout.write(buf, block_size);
  cout << "\" be changed to:" << "\n" << flush;
  delete[] buf;

  gxBlockHeader block_header;
  if(server->ReadHeader(block_header) != 0) return 1;

  char *block = new char[(__ULWORD__)block_header.block_length];
  if(server->ReadBlock((char *)block, block_header) != 0) {
    delete[] block;
    return 1;
  }

  cout << "Bytes = " << block_header.block_length << "\n" << flush;
  cout.write(block, block_header.block_length);
  cout.flush();

  delete[] block;
  return 0;
}

int AddBlock(gxSerialCommServer *server, gxBlockHeader &gx)
{
  cout << "Received an add block request" << "\n" << flush;
  __ULWORD__ block_size = (__ULWORD__)gx.block_length;

  char *block = new char[block_size];
  if(server->ReadBlock(block, gx) != 0) {
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

int DeleteBlock(gxSerialCommServer *server, gxBlockHeader &gx)
{
  cout << "Received a delete block request" << "\n" << flush;
  
  __ULWORD__ block_size = (__ULWORD__)gx.block_length;
  char *buf = new char[block_size];
  if(server->ReadBlock(buf, gx) != 0) {
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
  if(argc < 2) {
    cout << "GX Serial port server" << "\n" << flush;
    cout << "Usage: " << argv[0] << " device name [baud rate]" << "\n" 
	 << flush;
    return 1;
  }

  gxSerialCommServer server;

  int baud_rate = 9600;
  if(argc > 2 && atoi(argv[2])) baud_rate = atoi(argv[2]);

  cout << "Initializing the GX serial port server..." << "\n" << flush;
  int rv = server.InitCommServer(argv[1], baud_rate);

  if(rv != gxSerialComm::scomm_NO_ERROR) {
    cout << server.SerialCommExceptionMessage() << "\n" << flush;
    return 1;
  }

  cout << "Opening GX serial port server on " << argv[1] << "\n" << flush;
    
  while(1) { // Block until the next read. 
    // Wait for a block header
    gxBlockHeader gx;
    if(server.ReadHeader(gx) != 0) {
      cout << server.SerialCommExceptionMessage() << "\n" << flush;
      return 1;
    }

    cout << "Client connecting on " << argv[1] << "\n" << flush;

    // Read the status byte to determine what to do with this block
    __ULWORD__ block_status = gx.block_status;
    __SBYTE__ status = (__SBYTE__)((block_status & 0xFF00)>>8);

    switch(status) { 
      // Process each block of data
      case gxAcknowledgeBlock:
	cout << "Received an acknowledge block command" << "\n" << flush;
	cout << "Sending acknowledgment" << "\n" << flush;
	if(server.WriteAckBlock() != 0)
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;	  
	break;

      case gxAddRemoteBlock:
	if(AddBlock(&server, gx) != 0) 
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;
	break;

      case gxChangeRemoteBlock:
	if(ChangeBlock(&server, gx) != 0) 
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;
	break;

      case gxRequestBlock:
	if(AnswerRequest(&server, gx) != 0) 
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;
	break;

      case gxDeleteRemoteBlock:
	if(DeleteBlock(&server, gx) != 0) 
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;
	break;

      case gxSendBlock :  
	if(ReadBlock(&server, gx) != 0) 
	  cout << server.SerialCommExceptionMessage() << "\n" << flush;
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

  server.Close();
  cout << "Exiting..." << "\n" << flush;
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
