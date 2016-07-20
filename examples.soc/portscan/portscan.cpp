// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: portscan.cpp
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

This is a test program demonstrating how to build network-monitoring 
utilities using the GX Socket library. This example tests for connectivity 
using the embedded ping library, connects to a host using a full connection, 
and any reports information contained in the services file about an open 
port. A full connection uses a three-way TCP/IP handshake, which is 
extremely accurate and can easily be detected, logged, and filtered. 
This program and any derivatives are solely intended to detect open ports 
for informational and statistical purposes on systems you are authorized 
to access. 
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
#include "string.h"
#include "gxsping.h"
#include "dfileb.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int FindServiceByPort(const char *fname, int port_num)
{
  char entry[df_MAX_LINE_LENGTH];

  DiskFileB infile(fname);
  if(!infile) {
    cout << "\n";
    cout << "Cannot open the " << fname << " file" << "\n";
    return 1; // Do not generate an error in main function
  }

  int found_service = 0;

  while(!infile.df_EOF()) {
    if(infile.df_GetLine(entry, df_MAX_LINE_LENGTH, '\n', 1) != 
       DiskFileB::df_NO_ERROR) {
      break; // Error reading from the disk file
    }
    if(strcmp(entry, "") == 0) continue;
    char service[df_MAX_LINE_LENGTH];
    int port;
    char protocol[df_MAX_LINE_LENGTH];
    char aliases[df_MAX_LINE_LENGTH];
    char comment[df_MAX_LINE_LENGTH];
    if(ParseServiceFileEntry(entry, service, &port, 
			     protocol, aliases, comment)) {
      if(port_num == port) {
	found_service = 1;
	cout << "\n";
	if(service[0] != 0) 
	  cout << "Name:     " << service << "\n";
	if(protocol[0] != 0) 
	  cout << "Protocol: " << protocol << "\n";
	if(port > 0) 
	  cout << "Port:     " << port << "\n";
	if(aliases[0] != 0) 
	  cout << "Aliases:  " << aliases << "\n";
	if(comment[0] != 0) 
	  cout << "Comment:  " << comment << "\n";
      }
    }
  }
  infile.df_Close();
  return found_service == 1;
}

void PrintHostInfo(const char *hostname, gxSocket *sock_lib)
{
  gxsInternetAddress *ialist;
  gxsHostNameInfo *hostnm = sock_lib->GetHostInformation((char *)hostname);

  if(!hostnm) {
    cout << "\n";
    cout << "Could not resolve hostname" << "\n";
    return;
  }

  cout << "\n";
  cout << "Hostname: " << hostnm->h_name << "\n";

  int i;
  for(i = 0; ; i++) {
    char *alias = hostnm->h_aliases[i];
    if(alias == 0) break;
    cout << "Host Alias: " << alias << "\n"; 
  }

  for(i = 0; ; i++) {
    ialist = (gxsInternetAddress *)hostnm->h_addr_list[i];
    if(ialist == 0) break;
    cout << "Host IP Address: " << inet_ntoa(*ialist) << "\n";
  }
  delete hostnm;
}

int PingHost(const char *hostname, int *status)
{
#ifdef __UNIX__
  // Raw sockets require root access on all UNIX platforms. Programs
  // that use raw sockets can be executed by non-root users if the
  // effective user ID of the executing process is set to the same as
  // the file's owner. In the case root must own the executable and
  // the effective User-ID or Group-ID on execution bit must be set
  // by root using the chmod command: "chmod u+s" or "chmod 4755"
  // Now the effective user ID of the executing process will be set to 
  // the same user ID of the file's owner. 
  if(geteuid() != 0) {
    cout << "You must be root to run this program" << "\n";
    return 0;
  }
#endif

  // Construct a raw socket;
  gxSocket raw_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 
		      (char *)hostname);
  if(!raw_socket) {
    cout << raw_socket.SocketExceptionMessage() << "\n";
    return 0;
  }

  gxsPing ping(&raw_socket);

  // Ping with a timeout value of one second
  if(ping.Ping(1) != gxSOCKET_NO_ERROR) {
    if(raw_socket.GetSocketError() == gxSOCKET_REQUEST_TIMEOUT) {
      *status = 0; // This host is not responding
      return 1; 
    }
    cout << raw_socket.SocketExceptionMessage() << "\n";
    return 0;
  }

  return *status = 1;
}

int main(int argc, char **argv)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif
  
  // Check the command line arguments
  if(argc < 3) {
    cout << "\n";
    cout << "Usage 1: " << argv[0] << " hostname port" << "\n";
    cout << "Usage 2: " << argv[0] << " hostname port services_file" << "\n";
    return 1;
  }

  char *hostname = argv[1];
  unsigned short port = (unsigned short)atoi(argv[2]);
  
#if defined (__WIN32__)
  // char *fname = "c:\\windows\\services"; // WIN95/98
  // char *fname = "c:\\winnt\\system32\\drivers\\etc\\services"; // WINNT
  char *fname = "c:\\windows\\system32\\drivers\\etc\\services"; // WINXP
#else
  char *fname = "/etc/services";
#endif
  if(argc == 4) fname = argv[3];

  gxSocket client(SOCK_STREAM, port, hostname);
  if(!client) {
    cout << client.SocketExceptionMessage() << "\n";
    return 1;
  }

  cout << "Contacting host: " << hostname << "\n";
  PrintHostInfo((char *)hostname, &client);
  int status;
  if(!PingHost(hostname, &status)) return 1;
  if(status == 0) {
    cout << "Host not responding." << "\n";
    return 1;
  }

  cout << "Scanning TCP port " << port << "\n";
  
  if(client.Connect() < 0) {
    cout << "TCP port " << port << " closed" << "\n";
  }
  else {
    cout << "TCP port " << port << " open" << "\n";
    cout << "Reading services file entry..." << "\n";
    if(!FindServiceByPort((const char *)fname, port)) {
      cout << "\n";
      cout << "Service type unknown" << "\n";
    }
  }
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
