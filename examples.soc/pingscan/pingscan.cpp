// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: pingscan.cpp
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
using the embedded ping library and reports hostname information. This 
program and any derivatives are solely intended to test connectivity for 
informational and statistical purposes on systems you are authorized to 
access.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <stdio.h>
#include "gxsping.h"
#include "gxip32.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int GetHostInfo(const char *hostname)
{
  gxSocket sock_lib;
  if(sock_lib.InitSocketLibrary() != 0) {
    cout << sock_lib.SocketExceptionMessage() << "\n";
    return 0;
  }

  gxsInternetAddress *ialist;
  gxsHostNameInfo *hostnm = sock_lib.GetHostInformation((char *)hostname);

  if(!hostnm) return 0;

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
  return 1;
}

int PingHost(const char *hostname, int *status)
{
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

  // Check the command line arguments
  if(argc != 2) {
    cout << "\n";
    cout << "Usage: " << argv[0] << " class C subnet" << "\n";
    cout << "Example: " << argv[0] << " 192.168.0" << "\n";
    return 1;
  }

  gxIP32 sub_net_address(argv[1]);

  if((sub_net_address.GetByte1() == 0) || (sub_net_address.GetByte2() == 0) ||
     (sub_net_address.GetByte3() == 0)) {
    cout << argv[1] << " is not a class C address" << "\n";
    return 1;
  }

  sub_net_address.SetByte4((unsigned char)255);
  char sbuf[ip32MaxStrLen];
  cout << "Exploring the " << sub_net_address.c_str(sbuf) << " subnet" << "\n";

  for(int i = 1; i < 255; i++) {
    sub_net_address.SetByte4((unsigned char)i);
    cout << "\n";
    cout << "Contacting host: " << sub_net_address.c_str(sbuf) << "\n";

    int has_dns_entry = GetHostInfo(sub_net_address.c_str(sbuf));
    int status;
    if(!PingHost(sub_net_address.c_str(sbuf), &status)) return 1;
    if(status == 0) {
      if(has_dns_entry) {
	cout << "Host not responding." << "\n";
      }
      else {
	cout << "Host does not exist." << "\n";
      }
    }
    else {
      if(!has_dns_entry) {
	cout << "Host name: Cannot resolve host name" << "\n";
	cout << "Host IP Address: " << sub_net_address.c_str(sbuf) << "\n";
      }
    }
  }

  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
