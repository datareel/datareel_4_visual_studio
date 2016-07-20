// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: udpscan.cpp
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

Program used to scan UPD ports on your subnet. This UDP scanner works 
by sending a null string to the server and waiting for the service to 
send back an error message. If the port is listening the service will 
send back an error message or ignore the datagram. If the port is not 
listening the operating system on the server will send back an ICMP 
port unreachable packet. NOTE: UDP is connectionless protocol and 
scanning for open UDP ports is not reliable, especially when scanning 
nodes on a another subnet, passing through a firewall or router. The 
ICMP port unreachable packet may get lost in transit or the operating 
system on the server may not send back an ICMP port unreachable packet 
if the service is not active. 

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

#include <string.h>
#include <stdlib.h>
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
  unsigned short port = (unsigned short) atoi(argv[2]);
  int timeout_secs = 1;

#if defined (__WIN32__)
  // char *fname = "c:\\windows\\services"; // WIN95/98
  // char *fname = "c:\\winnt\\system32\\drivers\\etc\\services"; // WINNT
  char *fname = "c:\\windows\\system32\\drivers\\etc\\services"; // WINXP
#else
  char *fname = "/etc/services";
#endif
  if(argc == 4) fname = argv[3];

  gxSocket client(SOCK_DGRAM, port, hostname);
  if(!client) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
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

  // Set up the send and receive buffers
  const int datagram_size = 1;
  char datagram[datagram_size];
  memset(datagram, 0, datagram_size); 
  const int rxbuf_size = 4096;
  char rxbuf[rxbuf_size];
  memset(rxbuf, 0, rxbuf_size);  


  cout << "Scanning UDP port " << port << "\n";

  // Use connect to ensure that the unreachable packet is received
  client.Connect();
      
  int rv = client.SendTo((char *)datagram, datagram_size);
  int port_open = 0;
  if(rv < 0) {
    cout << client.SocketExceptionMessage() << "\n" << flush;
    return 1;
  }

  rv = client.RecvFrom(rxbuf, rxbuf_size, timeout_secs, 0);
  if(client.GetSocketError() == gxSOCKET_RECEIVE_ERROR) {
    cout << "UDP port " << port << " closed" << "\n";
  }
  else {
    if(rxbuf[0] != 0) { // The server sent back an error message
      port_open = 1;
    }
    if(client.GetSocketError() == gxSOCKET_REQUEST_TIMEOUT) {
      // The request timed out but the port may be closed if the 
      // ICMP port unreachable packet is not received before this 
      // error condition is reached.
      port_open = 1; // Assume the port is open
    }
    if(port_open) {
      cout << "UDP port " << port << " open" << "\n";
      cout << "Reading services file entry..." << "\n";
      if(!FindServiceByPort((const char *)fname, port)) {
	cout << "\n";
	cout << "Service type unknown" << "\n";
      }
    }
    else {
      cout << "UDP port " << port << " closed" << "\n";
    }
  }
 
  client.Close(); // Close the socket connection
  client.ReleaseSocketLibrary();
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

