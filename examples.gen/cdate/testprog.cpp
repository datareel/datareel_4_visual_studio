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

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
USA

This is a test program for the CDate class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "cdate.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  CDate cdate;

  cout << "Enter a date: " << "\n";
  cin >> cdate;

  cout << "\n";
  cout << "Testing overloaded << operator..." << "\n";
  cout << "You Entered: " << cdate << "\n";

  cout << "\n";
  cout << "Testing c_str() function..." << "\n";
  char *buf = cdate.c_str();
  cout << "You Entered: " << buf << "\n";
  delete buf; // Free the memory allocated for the string

  cout << "\n";
  char sbuf[255];
  cout << "Enter a date string MM/DD/YYYY: ";
  cin >> sbuf;
  
  if(!cin) { 
    cout << "Bad input string" << "\n";
  }
  else {
    if(!cdate.SetDate(sbuf)) {
      cout << "Bad input value" << "\n";
    }
    else {
      buf = cdate.c_str();
      cout << "You Entered: " << buf << "\n";
      delete buf; // Free the memory allocated for the string
    }
  }

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
