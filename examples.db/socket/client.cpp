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

This program is used to demonstrate how to stream database blocks
across a network connection.
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
#include "gxstream.h"
#include "dbobject.h"

void ClearInputStream(istream &s)
// Used to clear istream
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

void InputName(const char *mesg, DatabaseObjectData &dbobject)
{
  cout << mesg;
  char buf[gxMaxNameLength];
  for(int i = 0; i < gxMaxNameLength; i++) buf[i] = 0;
  cin >> buf;
  strcpy(dbobject.name, buf);
}

void DisplayDatabaseObject(DatabaseObjectData &dbobject, int full = 1)
{
  cout << "\n";
  cout << "Database object name: " << dbobject.name << "\n";
  if(full) {
    cout << "Database object OID:  " << dbobject.oid << "\n";
    cout.setf(ios::showpoint | ios::fixed);
    cout.precision(3);
    cout << "Database object CID:  " << dbobject.cid << "\n";
    cout << "\n";
  }
}

void PrgMenu()
{
  cout << "\n";
  cout << "(a, A)    Add an object to the database" << "\n";
  cout << "(c, C)    Change an object in the database" << "\n";
  cout << "(d, D)    Delete an object in the database" << "\n";
  cout << "(f, F)    Find an object in the database" << "\n";
  cout << "(h, H, ?) Help (prints this menu)" << "\n";
  cout << "(k, K)    Shutdown the server" << "\n";
  cout << "(q, Q)    Quit this program" << "\n";
  cout << "\n";
}

void DeleteObject(int port, char *host)
{
  gxStream client;
  if(client.StreamClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }

  cout << "Deleting an object from the database" << "\n";
  DatabaseObjectData dbobject;
  ClearInputStream(cin);
  InputName("Enter the name of the object to delete: ", dbobject);

  gxBlockHeader blk;
  blk.block_length = sizeof(DatabaseObjectData);

  if(client.DeleteBlock((char *)&dbobject, blk) != 0) 
    cout << client.SocketExceptionMessage() << "\n";

  client.Close();
}

void AddObject(int port, char *host)
{
  gxStream client;
  if(client.StreamClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }

  DatabaseObjectData dbobject;
  ClearInputStream(cin);
  InputName("Enter the name of the new object: ", dbobject);
  int id;
  double cid;

  cout << "Enter the object's id number: ";
  cin >> id;
  if(cin) {
    dbobject.oid = id;
    cout << "Enter the object's class id number: ";
    cin >> cid;
  } 
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  if(cin) {
    dbobject.cid = cid;
  }
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  
  ClearInputStream(cin);

  cout << "Sending a request to add the object" << "\n";

  gxBlockHeader blk;
  blk.block_length = sizeof(DatabaseObjectData);

  if(client.AddBlock((char *)&dbobject, blk) != 0) 
    cout << client.SocketExceptionMessage() << "\n";

  client.Close();
}

void ChangeObject(int port, char *host)
{
  gxStream client;
  if(client.StreamClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }

  cout << "Changing and object in the database" << "\n";
  DatabaseObjectData dbobject;
  ClearInputStream(cin);
  InputName("Enter the name of the object to change: ", dbobject);

  DatabaseObjectData new_dbobject_info;
  InputName("Enter the object's new name: ", new_dbobject_info);
  int id;
  double cid;

  cout << "Enter the object's new ID number: ";
  cin >> id;
  if(cin) {
    new_dbobject_info.oid = id;
    cout << "Enter the object's new class ID: ";
    cin >> cid;
  } 
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  if(cin) {
    new_dbobject_info.cid = cid;
  }
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  
  ClearInputStream(cin);
  
  gxBlockHeader request_header;
  request_header.block_length = sizeof(DatabaseObjectData);

  gxBlockHeader block_header;
  block_header.block_length = sizeof(DatabaseObjectData);

  if(client.ChangeBlock((char *)&dbobject, (char *)&new_dbobject_info, 
			request_header, block_header) != 0) 
    cout << client.SocketExceptionMessage() << "\n";

  client.Close();
}

void ShutDownServer(int port, char *host)
{
  gxStream client;
  if(client.StreamClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }

  cout << "Shutting down the server..." << "\n";
  if(client.TerminateConnection() != 0) 
    cout << client.SocketExceptionMessage() << "\n";
  client.Close();
}

void FindObject(int port, char *host)
{
  gxStream client;
  if(client.StreamClient(port, host) != 0) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }

  cout << "Searching the database" << "\n";
  DatabaseObjectData dbobject;
  ClearInputStream(cin);
  InputName("Enter the name of the object to find: ", dbobject);

  gxBlockHeader request_header;
  request_header.block_length = sizeof(DatabaseObjectData);
  gxBlockHeader requested_block_header;
  request_header.block_length = sizeof(DatabaseObjectData);

  DatabaseObjectData *p = \
    (DatabaseObjectData *)client.RequestBlock((char *)&dbobject,
					      request_header,
					      requested_block_header);
  if(!p) {
    cout << client.SocketExceptionMessage() << "\n";
    client.Close();
    return;
  }
  cout << "Server has answered request" << "\n";
  if(p->name[0] == 0) {
    cout << "The dbobject was not found in the database" << "\n";
  }
  else {
    cout << "Found the requested object: " << "\n";
    DisplayDatabaseObject(*(p));
  }

  delete p;
  client.Close();
}

int main(int argc, char **argv)
{
  if(argc < 3) {
    cerr << "Usage: " << argv[0] << " hostname port" << "\n";
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
	cout << "Input stream error" << "\n";
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
	AddObject(port, host);
	break;
      case 'c' : case 'C' : 
	ChangeObject(port, host);
	break;
      case 'd' : case 'D' :  
	DeleteObject(port, host);
	break;
      case 'f' : case 'F' :
	FindObject(port, host);
	break;
      case 'k' : case 'K' : 
	ShutDownServer(port, host);
	break;
      default:
        cout << "Unrecognized command" << "\n";
    }
  }
 
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
