// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: arpscan.cpp
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

Test program used to get the Media Access Control (MAC) addresses
of all the hosts on a specified class C subnet.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
#include <fstream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#include <fstream.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gxsping.h"
#include "gxip32.h"
#include "gxmac48.h"

// Platform specific include files
#if defined (__UNIX__)
#include <net/if_arp.h>
#include <sys/ioctl.h>
#endif

#if defined (__WIN32__)
int WIN32GetMAC(const char *ip_address, char *mac_address);
#endif

int GetMacAddress(const char *ip_address, char *eth_dev, char *fname = 0,
		  fstream *outfile = 0)
{
  // Construct a raw socket;
  gxSocket raw_socket;
  if(raw_socket.InitSocketLibrary() != 0) {
    cout << raw_socket.SocketExceptionMessage() << "\n" << flush;
    if(outfile)
      *(outfile) << raw_socket.SocketExceptionMessage() << "\n" << flush;
    return 0;
  }
  cout << ip_address << '\t' << flush;
  if(outfile) *(outfile) << ip_address << '\t' << flush;
  gxsHostNameInfo *hostnm = raw_socket.GetHostInformation((char *)ip_address);
  if(hostnm) {
    cout << hostnm->h_name << flush;
    if(outfile) *(outfile) << hostnm->h_name << flush;
  }
  else {
    cout << "no_DNS_entry" << flush;
   if(outfile) *(outfile) << "no_DNS_entry" << flush;
  }
  
  if(raw_socket.InitSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 
			   (char *)ip_address) < 0) {
    cout << raw_socket.SocketExceptionMessage() << "\n" << flush;
    if(outfile)
      *(outfile) << raw_socket.SocketExceptionMessage() << "\n" << flush;
    return 0;
  }
  
  // Ping with a specified timeout value
  gxsPing ping(&raw_socket);
  if(ping.Ping(4) != gxSOCKET_NO_ERROR) {
    if(raw_socket.GetSocketError() == gxSOCKET_REQUEST_TIMEOUT) {
    }
    else {
      cout << "\n" << flush;
      cout << raw_socket.SocketExceptionMessage() << "\n" << flush;
      if(outfile) {
	*(outfile) << "\n" << flush;
	*(outfile) << raw_socket.SocketExceptionMessage() << "\n" << flush;
      }
      delete hostnm;
      return 0;
    }
  }

#if defined (__WIN32__)
  char mac_address[255];
  if(!WIN32GetMAC(ip_address, mac_address)) {
    cout << "\n" << flush;
    if(outfile) *(outfile) << "\n" << flush;
    delete hostnm;
    return 0; // No entry in the arp cache
  } 
  gxMAC48 mac(mac_address, '-', 2);
  cout << '\t' << mac.c_str(mac_address) << flush;
  if(outfile) *(outfile) << '\t' << mac.c_str(mac_address) << flush;
#elif defined (__UNIX__)
  arpreq arp_data;
  memset(&arp_data, 0, sizeof(arp_data));
  memcpy(&arp_data.arp_pa, &raw_socket.sin, sizeof(arp_data.arp_pa));         

#if defined (__LINUX__) 
  strcpy(arp_data.arp_dev, eth_dev);
#endif
  
#ifndef SIOCGARP
#error SIOCGARP is not available on this platform
#endif

  if(ioctl(raw_socket.GetSocket(), SIOCGARP, &arp_data) == -1) {
    cout << "\n" << flush;
    if(outfile) *(outfile) << "\n" << flush; 
    delete hostnm;
    return 0; // No entry in the arp cache
  }
  
  char mac_address[255];
  unsigned char *ptr = (unsigned char *)&arp_data.arp_ha.sa_data[0];
  sprintf(mac_address, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",*ptr, 
	  *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
  
  gxMAC48 mac(mac_address, ':', 2);
  cout << '\t' << mac.c_str(mac_address) << flush;
  if(outfile) *(outfile) << '\t' << mac.c_str(mac_address) << flush;
#else
#error You must define a target platform: __WIN32__ or __UNIX__
#endif

  if(fname) { // Find the vendor information
    char name[255];
    char comment[255];
    if(FindVendor((const char *)fname, mac, name, comment)) {
      cout << '\t' << name << flush;
      if(outfile) *(outfile) << '\t' << name << flush;
    }
  }
  cout << "\n" << flush;
  if(outfile) *(outfile) << "\n" << flush;
  delete hostnm;
  return 1;
}

int main(int argc, char **argv)
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

  // Check the command line arguments
#if defined (__WIN32__)
  if(argc < 2) {
    cout << "\n";
    cout << "Usage: " << argv[0] << " subnet " << "\n";
    cout << "Example 1: " << argv[0] <<" 192.168.0 " << "\n";
    cout << "Example 2: " << argv[0] <<" 192.168.0 maccodes.txt" << "\n";
    cout << "Example 3: " << argv[0] <<" 192.168.0.1 " << "\n";
    cout << "Example 4: " << argv[0] <<" 192.168.0.1 maccodes.txt" << "\n";
    cout << "Example 5: " << argv[0] <<" 192.168.0.255 maccodes.txt \
outfile.txt" << "\n";
    return 1;
  }
  char *subnet = argv[1];
  char *eth_dev = 0;
  char *fname = 0;
  char *ofname = 0;
  if(argc >= 3) fname = argv[2];
  if(argc == 4) ofname = argv[3];
#elif defined (__UNIX__)
  if(argc < 3) {
    cout << "\n";
    cout << "Usage: " << argv[0] << " subnet ethernet_device" << "\n";
    cout << "Example 1: " << argv[0] <<" 192.168.0 eth0" << "\n";
    cout << "Example 2: " << argv[0] <<" 192.168.0 eth0 maccodes.txt" 
	 << "\n";
    cout << "Example 3: " << argv[0] <<" 192.168.0.1 eth0" << "\n";
    cout << "Example 4: " << argv[0] <<" 192.168.0.1 eth0 maccodes.txt" 
	 << "\n";
    cout << "Example 5: " << argv[0] <<" 192.168.0.255 eth0 maccodes.txt \
outfile.txt" << "\n";
    return 1;
  }
  char *subnet = argv[1];
  char *eth_dev = argv[2];
  char *fname = 0;
  char *ofname = 0;
  if(argc >= 4) fname = argv[3];
  if(argc == 5) fname = argv[4];
#else
#error You must define a target platform: __WIN32__ or __UNIX__
#endif

  fstream outfile;
  int out = 0;
  if(ofname) {
    // Open the output file and truncate if it already exists
    out = 1;
    outfile.open(ofname, ios::out|ios::trunc|ios::binary);
    if(!outfile) {
      cout << "Could not open the " << ofname << " output file" << "\n";
      return 1;
    }
  }

  gxIP32 ip32(subnet);
  char sbuf[255];
  if((ip32.GetByte4() == 0) || (ip32.GetByte4() == 255)) {
    // Find the mac address of all the nodes on a specified subnet
    for(int i = 1; i < 255; i++) {
      ip32.SetByte4(i);
      if(out)
	GetMacAddress(ip32.c_str(sbuf), eth_dev, fname, &outfile); 
      else
	GetMacAddress(ip32.c_str(sbuf), eth_dev, fname);
    }
  }
  else {
    // Get the mac address of a single node
    if(out)
      GetMacAddress(ip32.c_str(sbuf), eth_dev, fname, &outfile);
    else 
      GetMacAddress(ip32.c_str(sbuf), eth_dev, fname);
  }

  if(out) outfile.close();
  return 0;
}

#if defined (__WIN32__)
int WIN32GetMAC(const char *ip_address, char *mac_address)
// Function used to get a mac address from the WIN32 arp cache.
// Returns true if an arp cache entry exists for the specified
// IP address string.
{
  system("arp -a > arpcache.txt");
  ifstream infile("arpcache.txt", ios::in);
  if(!infile) return 0; // Cannot open the file
  
  char sbuf[255];
  char ip_in[255];

  while(!infile.eof()) {
    infile.getline(sbuf, 255);
    int len, offset, i = 0;
    int byte_count = strlen(sbuf);
    ip_in[0] = 0;
    mac_address[0] = 0;
    
    // Trim leading tabs and spaces
    while((sbuf[i] == ' ') || (sbuf[i] == '\t')) i++;
    byte_count -= i;
    if(byte_count <= 0) continue;
    memmove(sbuf, sbuf+i, byte_count);
    sbuf[byte_count] = 0;

    // Parse the IP Address
    for(i = 0; i < byte_count; i ++) {
      if((sbuf[i] == ' ') || (sbuf[i] == '\t')) break;
      ip_in[i] = sbuf[i];
    }
    ip_in[i] = 0; // Null terminate the string

    if(strcmp(ip_in, ip_address) == 0) { // Parse the MAC Address
      while((sbuf[i] == ' ') || (sbuf[i] == '\t')) i++;
      byte_count -= i;
      if(byte_count <= 0) continue;
      memmove(sbuf, sbuf+i, byte_count);
      for(i = 0; i < byte_count; i ++) {
	if(sbuf[i] == ' ') break;
	mac_address[i] = sbuf[i];
      }
      mac_address[i] = 0; // Null terminate the string
      
      if(mac_address[0] != 0)  { // Remove any trailing spaces
	offset = len = strlen(mac_address);
	while(offset--) if(mac_address[offset] != ' ') break;
	if(offset < len) mac_address[offset+1] = 0;
	return 1; // Found the arp cache entry
      }
      else {
	return 0;
      }
    }
  }
  infile.close();
  return 0;
}
#endif
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
