// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp 
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 10/15/1999 
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

Code used to test encoded data sets. EDS version 2

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
#include <iomanip>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#include <iomanip.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include <stdlib.h>
#include "eds201.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void PrintEDSTable()
{
  eds2String eds;
  for(unsigned i = 0; i < eds2TableSize; i++) 
  cout << setfill('0') << setw(4) << hex << eds.eds2_table[i] << ' ';
  cout << "\n";
}

void EncodeString(eds2WORD_t magic_number, const char *s)
{
  char *s1 = (char *)s;
  int len = strlen(s1);
  int i = 0;
  eds2String eds(magic_number);
  
  // Encode the string
  eds2WORD *eds_string = new eds2WORD[len];
  for(i = 0; i < len; i++) {
    eds_string[i] = eds.EncodeString((unsigned char)*s1++);
    cout << setfill('0') << setw(4) << hex << eds_string[i] << ' ';
  }
  cout << "\n";

  // Decode the string
  int eds_len = len * sizeof(eds2WORD);
  int str_len = int((__LWORD__)eds_len/(__LWORD__)sizeof(eds2WORD));
  char *str = new char[str_len+1];
  for(i = 0; i < str_len; i++) {
    unsigned char c;
    str[i] = 0;
    if(eds.DecodeString(eds_string[i], c)) {
      str[i] = c;
    }
  }
  str[str_len] = 0; // Ensure null termination
  cout << str;
  cout << "\n";

  delete[] eds_string;
  delete[] str;
}

int main(int argv, char **argc)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  eds2WORD_t magic_number = eds2MagicNumber;   
  if(argv == 2) magic_number = atoi(argc[1]);

  const char *s1 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789\n";
  const char *s2 = "the quick brown fox jumps over the lazy dog 0123456789\n";
  const char *s3 = "!@#$%^&*()-_=+[{]}\\|;:\'\",<.>/?`~\n";
  const char *s4 = "\t\n\r\b";
  
  cout << "\n";
  cout << "Encoding/Decoding string number 1..." << "\n";
  EncodeString(magic_number, s1);
  
  cout << "Encoding/Decoding string number 2..." << "\n";
  EncodeString(magic_number, s2);
  
  cout << "Encoding/Decoding string number 3..." << "\n";
  EncodeString(magic_number, s3);
    
  cout << "Encoding/Decoding string number 4..." << "\n";
  EncodeString(magic_number, s4);

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
