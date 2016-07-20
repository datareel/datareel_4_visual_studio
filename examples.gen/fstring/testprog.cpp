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

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
USA

This is a test program for the FString class. 
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "fstring.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  FString sz;
  cout << "Size of FString object = " << sz.SizeOf() << "\n";
 
  cout << "Constructing FString class objects..." << "\n";
  FString a("DOG");
  FString b("CAT");
  FString c("MOUSE");
  FString d("BIRD");

  cout << "Testing copy constructor..." << "\n";
  FString e(a);  

  cout << "Testing assignment operator..." << "\n";
  a = b;

  cout << "\n";
  cout << "Testing overloaded << operator..."  << "\n";
  cout << a << ' ' << b  << ' ' << c  << ' ' << d << ' ' << e << "\n";

  cout << "\n";
  cout << "Testing c_str() function..." << "\n";
  cout << a.c_str() << "\n";
  cout << b.c_str() << "\n";
  cout << c.c_str() << "\n";
  cout << d.c_str() << "\n";
  cout << e.c_str() << "\n";

  cout << "\n";
  cout << "Testing overloaded >> operator..." << "\n";
  FString buf;
  cout << "Enter a string up to " << FStringLength << " characters long: ";
  cin >> buf;

  cout << "\n";
  cout << "Data entered = " << buf << "\n";

  cout << "\n";
  cout << "Testing FString boundaries..." << "\n";
  char *s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789";
  FString x(s);
  cout << "Alphabet string = " << x.c_str() << "\n";

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
