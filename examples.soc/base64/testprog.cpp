// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 01/25/2000
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

This is a test program for the base 64 encode and decode
routines.

NOTE: Data encoding is a very weak form of asymmetric encryption 
and can be cracked more readily than stronger forms of asymmetric 
encryption. The data encoding routines provided with this library 
should not be implemented in any measure to stop malicious hackers 
on the Internet from breaking into communication systems or from 
compromising encoded data files. The data encoding routines are an 
alternative to sending plain text messages or writing plain text to 
database or configuration files. These routines are simple, fast, 
and integrated into the library so you do not need to include any 
other third party libraries when building code using this library. 
If you need a comprehensive strong encryption toolkit please visit 
the http://www.openssl.org Website to download the latest version 
of OpenSSL.      
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxs_b64.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void InputString(const char *mesg, gxString &s)
{
  cout << mesg;
  char sbuf[255];
  for(int i = 0; i < 255; i++) sbuf[i] = 0;
  cin >> sbuf;
  s = sbuf;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  cout << "\n";
  gxString wwwauth, ubuf, pbuf, user, passwd;
  cout << "Base 64 encoding username and password" << "\n";
  InputString("Username: ", ubuf);
  InputString("Password: ", pbuf);
  cout << "\n";
  
  gxsBasicAuthenticationEncode(ubuf.c_str(), pbuf.c_str(), "Basic", wwwauth);
  cout << wwwauth << "\n";
  cout << "Decoding base 64 authentication line" << "\n";
  gxsBasicAuthenticationDecode(wwwauth, user, passwd);
  cout << "Username: " << user << "\n";
  cout << "Password: " << passwd << "\n";
  cout << "\n";
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
