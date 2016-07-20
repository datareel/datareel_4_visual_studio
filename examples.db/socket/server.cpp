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
#include "gxstream.h"
#include "dbobject.h"

int FindObject(gxStream *server, gxBlockHeader &blk,
	       DatabaseObject *dbobjectdb)
{
  cout << "Client has queried the database" << "\n" << flush;
 
  if(server->ReadRemoteBlock((char *)dbobjectdb->p, blk) != 0) return 1;
  cout << "Searching for: " << dbobjectdb->p->name << "\n" << flush;
  
  FAU_t addr = dbobjectdb->FindObject();
  if(addr == (FAU_t)0) {
    cout << "Did not find the object" << "\n" << flush;
    dbobjectdb->p->name[0] = 0;
    dbobjectdb->p->oid = (OID)0;
    dbobjectdb->p->cid = (CID)0;
  }
  else
    cout << "Found the requested object" << "\n" << flush;

  cout << "Sending search results to the client" << "\n" << flush;
  if(server->WriteRemoteBlock((char *)dbobjectdb->p,
			      sizeof(DatabaseObjectData))
     != 0) return 1;

  return 0;
}

int ChangeObject(gxStream *server, gxBlockHeader &blk,
		 DatabaseObject *dbobjectdb)
{
  cout << "Received a change object request" << "\n" << flush;
 
  // Read the name of the object to be changed
  if(server->ReadRemoteBlock((char *)dbobjectdb->p, blk) != 0) return 1;

  cout << "Client requested a change to object: " <<
    dbobjectdb->p->name << "\n" << flush;

  DatabaseObjectData new_dbobject_info;
  gxBlockHeader block_header;

  // Get the object's new info from the client 
  if(server->ReadClientHeader(block_header) != 0) return 1;
  if(server->ReadRemoteBlock((char *)&new_dbobject_info, block_header) != 0)
    return 1;

  FAU_t addr = dbobjectdb->ChangeObject(new_dbobject_info);
  if(addr == (FAU_t)0) {
    cout << "Change request denied: Did not find object in the database" 
	 << "\n" << flush;
    dbobjectdb->p->name[0] = 0;
    dbobjectdb->p->oid = (OID)0;
    dbobjectdb->p->cid = (CID)0;
  }
  else {
    cout << "Object changed to: " << dbobjectdb->p->name << ", "
	 << dbobjectdb->p->oid 
	 << ", " << dbobjectdb->p->cid << "\n" << flush;
  }
  return 0;
}

int AddObject(gxStream *server, gxBlockHeader &blk,
	      DatabaseObject *dbobjectdb)
{
  cout << "Adding an object" << "\n" << flush;
  if(server->ReadRemoteBlock(dbobjectdb->p, blk) != 0) return 1;
  dbobjectdb->WriteObject();
  cout << dbobjectdb->p->name << " added to the database" << "\n" << flush;
  return 0;
}

int DeleteObject(gxStream *server, gxBlockHeader &blk,
		 DatabaseObject *dbobjectdb)
{
  cout << "Deleting an object" << "\n" << flush;
  if(server->ReadRemoteBlock(dbobjectdb->p, blk) != 0) return 1;
  FAU_t addr = dbobjectdb->DeleteObject();
  if(addr == (FAU_t)0) { 
    cout << "Could not find \"" << dbobjectdb->p->name << "\" in the database" 
	 << "\n" << flush;
  }
  else {
    cout << "Deleted: " << dbobjectdb->p->name << "\n" << flush;
  }
  return 0;
}

int main(int argc, char **argv)
{
  // Check arguments. Should be only one: the port number to bind to.
  if(argc != 2) {
    cerr << "Usage: " << argv[0] << " port" << "\n" << flush;
    return 1;
  }
  
  DatabaseObject *dbobjectdb = new DatabaseObject; 
  cout << "Initializing the database..." << "\n" << flush;
  const char *fname = "dbobject.gxd";

  if(!gxDatabase::Exists(fname)) {
    cout << "Creating new file..." << "\n" << flush;
    dbobjectdb->Create(fname);
  }
  else {
    cout << "Opening existing file..." << "\n" << flush;
    dbobjectdb->Open(fname);
  }

  gxStream server;
  gxsSocket_t remote_socket;
  unsigned short port = (unsigned short) atoi(argv[1]);

  cout << "Initializing the database stream server..." << "\n" << flush;
  if(server.StreamServer(port) != 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    delete dbobjectdb;
    return 1;
  }
  
  // Get the host name assigned to this machine
  char hostname[gxsMAX_NAME_LEN];
  if(server.HostName(hostname) != 0) {
    cout << server.SocketExceptionMessage() << "\n" << flush;
    delete dbobjectdb;
    return 1;
  }
  cout << "Opening database stream server on host " << hostname 
       << "\n" << flush;

  int server_up = 1;
  while(server_up) { // Loop until signaled to exit 
    cout << "Listening on port " << port << "\n" << flush;
    remote_socket = server.Accept(); // Block until the next read.
    if(remote_socket < 0) {
      cout << server.SocketExceptionMessage() << "\n" << flush;
      delete dbobjectdb;
      return 1;
    }

    // Read the block following a client connection
    gxBlockHeader blk;
    if(server.ReadClientHeader(blk) != 0) {
      cout << server.SocketExceptionMessage() << "\n" << flush;
      delete dbobjectdb;
      return 1;
    }

    cout << flush;
    
    // Get the client info
    char client_name[gxsMAX_NAME_LEN]; int r_port = -1;
    server.GetClientInfo(client_name, r_port);
    cout << client_name << " connecting on port " << r_port << "\n" << flush;

    // Read the status byte to determine what to do with this block
    __ULWORD__ block_status = blk.block_status;
    __SBYTE__ status = (__SBYTE__)((block_status & 0xFF00)>>8);

    switch(status) { 
      // Process each block of data
      case gxAcknowledgeBlock:
	cout << "Received an acknowledge block command" << "\n" << flush;
	cout << "Sending acknowledgment" << "\n" << flush;
	if(server.WriteRemoteAckBlock() != 0)
	  cout << server.SocketExceptionMessage() << "\n" << flush;	  
	server.CloseRemoteSocket();
	break;

      case gxAddRemoteBlock:
        if(AddObject(&server, blk, dbobjectdb) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxChangeRemoteBlock:
        if(ChangeObject(&server, blk, dbobjectdb) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxRequestBlock:
        if(FindObject(&server, blk, dbobjectdb) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxDeleteRemoteBlock:
        if(DeleteObject(&server, blk, dbobjectdb) != 0) 
	  cout << server.SocketExceptionMessage() << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxSendBlock :  
	cout << "Not excepting raw data blocks" << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxCloseConnection : 
	cout << "Client sent a close connection command" << "\n" << flush;
	server.CloseRemoteSocket();
	break;

      case gxKillServer:
	cout << "Client shutdown the server" << "\n" << flush;
	server_up = 0;
	break;
	
      default:
	cout << "Received bad block command from client" << "\n" << flush;
	server.CloseRemoteSocket();
	break;
    }
  }

  dbobjectdb->Close();
  delete dbobjectdb;
  
  server.Close();
  cout << "Exiting..." << "\n" << flush;
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
