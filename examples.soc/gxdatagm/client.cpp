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

#include <stdlib.h>
#include <string.h>
#include "gxdatagm.h"

const char *test_block = "The quick brown fox jumps over the lazy dog \
0123456789\n";

void ClearInputStream(istream &s)
// Used to clear istream
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

void PrgMenu()
{
  cout << "\n" << flush;
  cout << "(a, A)    Add a database block" << "\n" << flush;
  cout << "(c, C)    Change a database block" << "\n" << flush;
  cout << "(d, D)    Delete a database block" << "\n" << flush;
  cout << "(l, L)    Send a client close command" << "\n" << flush;
  cout << "(h, H, ?) Help (prints this menu)" << "\n" << flush;
  cout << "(k, K)    Shutdown the server" << "\n" << flush;
  cout << "(q, Q)    Quit this program" << "\n" << flush;
  cout << "(r, R)    Request a database block" << "\n" << flush;
  cout << "(s, S)    Send a raw block of data" << "\n" << flush;
  cout << "(w, W)    Send an acknowledgment block" << "\n" << flush;
  cout << "\n" << flush;
}

void Acknowledge(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Requesting acknowledgment block from the server" << "\n" << flush;

  if(client.WriteAckBlock() != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  if(client.ReadAckBlock() != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Received an acknowledgment from the server " << "\n" << flush;
  client.Close();
}

void CloseCommand(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }
  cout << "Closing the connection..." << "\n" << flush;
  if(client.CloseConnection() != 0)
    cout << client.SocketExceptionMessage() << "\n" << flush;
  client.Close();
}

void DeleteBlock(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Sending a request to delete a remote block" << "\n" << flush;
  char *request = "DOG"; // Arbitrary block ID
  gxBlockHeader gx;
  gx.block_length = strlen(request);

  if(client.DeleteBlock((char *)request, gx) != 0) 
    cout << client.SocketExceptionMessage() << "\n" << flush;

  client.Close();
}

void AddBlock(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Sending a request to add a remote block" << "\n" << flush;

  gxBlockHeader gx;
  gx.block_length = strlen(test_block);

  if(client.AddBlock((char *)test_block, gx) != 0) 
    cout << client.SocketExceptionMessage() << "\n" << flush;

  client.Close();
}

void ChangeBlock(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Sending a request to change a remote block" << "\n" << flush;

  char *request = "CAT"; // Arbitrary block ID
  gxBlockHeader request_header;
  request_header.block_length = strlen(request);

  gxBlockHeader block_header;
  block_header.block_length = strlen(test_block);

  if(client.ChangeBlock((char *)request, (char *)test_block, request_header,
			block_header) != 0) 
    cout << client.SocketExceptionMessage() << "\n" << flush;

  client.Close();
}

void ShutDownServer(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Shutting down the server..." << "\n" << flush;
  if(client.TerminateConnection() != 0) 
    cout << client.SocketExceptionMessage() << "\n" << flush;
  client.Close();
}

void RequestBlock(int port, char *host)
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Requesting a block from the server" << "\n" << flush;
  char *request = "FOX"; // Arbitrary block ID
  gxBlockHeader request_header;
  request_header.block_length = strlen(request);
  gxBlockHeader requested_block_header;
  request_header.block_length = strlen(request);

  void *block = client.RequestBlock((char *)request, request_header,
				    requested_block_header);
  if(!block) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }
  cout << "Received requested block" << "\n" << flush;
  cout.write((char *)block, requested_block_header.block_length);
  
  client.Close();
}

void SendBlock(int port, char *host)
// Send a block of data
{
  gxDatagram client;
  if(client.DatagramClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }

  cout << "Sending a block " << strlen(test_block) << " bytes long..." 
       << "\n" << flush;
  if(client.WriteBlock((char *)test_block, strlen(test_block)) != 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    client.Close();
    return;
  }
  client.Close();
}

int main(int argc, char **argv)
{
  if(argc < 3) {
    cerr << "Usage: " << argv[0] << " hostname port" << "\n" << flush;
    return 1;
  }

  unsigned short port = (unsigned short) atoi(argv[2]);
  char *host = argv[1];

  char key;
  PrgMenu();
  int rv = 1;
  while(rv) {
    if (!cin) {
      ClearInputStream(cin);
      if (!cin) {
	cout << "Input stream error" << "\n" << flush;
	return 0;
      }
    }
    cout << '>';
    cin >> key;
    if (!cin) continue;
    switch(key) {
      case '?' : PrgMenu(); break;
      case 'h' : case 'H' : PrgMenu(); break;
      case 'q' : case 'Q' : rv = 0; break;

      case 'a' : case 'A' : 
	AddBlock(port, host);
	break;
      case 'c' : case 'C' : 
	ChangeBlock(port, host);
	break;
      case 'd' : case 'D' :  
	DeleteBlock(port, host);
	break;
      case 'k' : case 'K' : 
	ShutDownServer(port, host);
	break;
      case 'r' : case 'R' : 
	RequestBlock(port, host);
	break;
      case 's' : case 'S' : 
	SendBlock(port, host);
	break;
      case 'l' : case 'L' : 
	CloseCommand(port, host);
	break;
      case 'w' : case 'W' : 
	Acknowledge(port, host);
	break;
      default:
        cout << "Unrecognized command" << "\n" << flush;
    }
  }
 
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
