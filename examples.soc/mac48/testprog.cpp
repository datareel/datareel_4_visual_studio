// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/21/1997
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

This is a test program for the gxMAC48 class. 
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxmac48.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main(int argc, char **argv)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  cout << "Testing gxMAC48 constructors..." << "\n";
  gxMAC48 mac1(0x00,0x60,0xb0,0xf9,0xb8,0x95);
  gxMAC48 mac2;

  mac2.SetByte1(0x00); mac2.SetByte2(0x60); mac2.SetByte3(0xb0); 
  mac2.SetByte4(0xf9); mac2.SetByte5(0xb8); mac2.SetByte6(0x95);

  char sbuf[mac48MaxStrLen];
  cout << "MAC address 1 = " << mac1.c_str(sbuf) << "\n";
  cout << "MAC address 2 = " << mac2.c_str(sbuf) << "\n";

  PausePrg();

  cout << "Testing overloaded assignment operator..." << "\n";
  gxMAC48 mac3;
  mac3 = mac1;
  cout << "MAC address 3 = " << mac3.c_str(sbuf) << "\n";

  PausePrg();

  cout << "Testing copy constructors..." << "\n";
  gxMAC48 mac4(mac2);
  gxMAC48 mac5("00:60:b0:f9:b8:95");

  cout << "MAC address 4 = " << mac4.c_str(sbuf) << "\n";
  cout << "MAC address 5 = " << mac5.c_str(sbuf) << "\n";

  PausePrg();

  cout << "Testing string input functions" << "\n";
  mac5.SetMACAddress("00:60:b0:f9:b8:95", ':', 2);
  cout << "MAC input string = 00:60:b0:f9:b8:95 output = " 
       << mac5.c_str(sbuf) << "\n";
  mac5.SetMACAddress("00-60-b0-f9-b8-95", '-', 2);
  cout << "MAC input string = 00-60-b0-f9-b8-95 output = " 
       << mac5.c_str(sbuf, '-') << "\n";
  mac5.SetMACAddress("00.60.b0.f9.b8.95", '.', 2);
  cout << "MAC input string = 00.60.b0.f9.b8.95 output = " 
       << mac5.c_str(sbuf, '.') << "\n";
  mac5.SetMACAddress("0060:b0f9:b895", ':', 4);
  cout << "MAC input string = 0060:b0f9:b895 output = " 
       << mac5.c_str(sbuf, ':', 4) << "\n";
  mac5.SetMACAddress("0060-b0f9-b895", '-', 4);
  cout << "MAC input string = 0060-b0f9-b895 output = " 
       << mac5.c_str(sbuf, '-', 4) << "\n";
  mac5.SetMACAddress("0060.b0f9.b895", '.', 4);
  cout << "MAC input string = 0060.b0f9.b895 output = " 
       << mac5.c_str(sbuf, '.', 4) << "\n";

  PausePrg();
  cout << "Testing the vendor code functions" << "\n";
  cout << "Vendor code = " << hex << mac5.GetVendorCode() << "\n";
  cout << "Vendor code string = " << mac5.GetVendorCode(sbuf) << "\n";

  // Find the vendor code if a MAC vendor text file is specified
  // on the command line
  if(argc == 2) {
    cout << "\n";
    cout << "Searching for vendor info..." << "\n";
    const char *fname = (const char *)argv[1];
    char name[255];
    char comment[255];
    if(FindVendor(fname, mac5, name, comment)) {
      cout << "Vendor name = " << name << "\n";
      if(comment[0] != 0) 
	cout << "Comments    = " << comment << "\n";
    }
    else {
      cout << "Could not obtain vendor info from the " 
	   << fname << " file" << "\n";
    }
  }
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
