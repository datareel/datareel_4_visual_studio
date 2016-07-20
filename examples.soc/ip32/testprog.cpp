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

This is a test program for the gxIP32 class. 
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxip32.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  cout << "Testing gxIP32 constructors..." << "\n";
  gxIP32 net1(127,0,0,1);
  gxIP32 net2;

  net2.SetByte1(127); net2.SetByte2(0); net2.SetByte3(0); net2.SetByte4(2);

  char sbuf[ip32MaxStrLen];
  cout << "IP address 1 = " << net1.c_str(sbuf) << "\n";
  cout << "IP address 2 = " << net2.c_str(sbuf) << "\n";

  PausePrg();

  cout << "Testing overloaded assignment operator..." << "\n";
  gxIP32 net3;
  net3 = net1;
  cout << "IP address 3 = " << net3.c_str(sbuf) << "\n";

  PausePrg();

  cout << "Testing copy constructors..." << "\n";
  gxIP32 net4(net2);
  gxIP32 net5("127.0.0.3");

  cout << "IP address 4 = " << net4.c_str(sbuf) << "\n";
  cout << "IP address 5 = " << net5.c_str(sbuf) << "\n";

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
