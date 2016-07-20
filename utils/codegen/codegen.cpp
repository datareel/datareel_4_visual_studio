// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: codegen.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
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

Code used to generate encoded software registration codes.

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
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include <stdio.h>
#include "sreg101.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Test code starts here
void ClearInputStream(GXSTD::istream &s)
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

void PausePrg()
{
  GXSTD::cout << "\n";
  GXSTD::cout << "Press enter to continue..." << "\n";
  GXSTD::cin.get();
}

void InputData(char *mesg, UString &sbuf)
{
  GXSTD::cout << "Input " << mesg << ": ";
  char buf[255];
  GXSTD::cin.getline(buf, sizeof(buf));
  sbuf = buf;
}

void PublishCode(char *pname, char *user_name, char *reg_code)
{
  GXSTD::cout << "\n";
  GXSTD::cout << "Your " << pname << " user name and registration code."
       << "\n";
  GXSTD::cout << "Assigned User Name: " << user_name << "\n";
  GXSTD::cout << "Registration Number: " << reg_code << "\n";
  GXSTD::cout << "\n";
}

void Menu()
{
  GXSTD::cout << "\n";
  GXSTD::cout.setf(GXSTD::ios::showpoint|GXSTD::ios::fixed);
  GXSTD::cout.precision(3);
  GXSTD::cout << "Registration code generator version: "
	      << srnVersionNumber << "\n";
  GXSTD::cout << "(?)   Display this menu" << "\n";
  GXSTD::cout << "(e)   Enter the program's name" << "\n";
  GXSTD::cout << "(g)   Generate a new registration code" << "\n";
  GXSTD::cout << "(u)   Generate codes for new users" << "\n";
  GXSTD::cout << "(v)   Validate an existing registration code" << "\n";
  GXSTD::cout << "(q)   Quit" << "\n";
  GXSTD::cout << "\n";
}

int Quit()
{
  return 0;
}

int main(int argc, char **argv)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  if(argc >= 3) { 
    srnGenerator auto_srn(argv[1], argv[2]);
    PublishCode(argv[1], argv[2], auto_srn.GetRegCode());
    return 0;
  }

  srnGenerator srn;
  int i, rv;
  char key;
  UString pname; UString user_name; UString reg_code;

  if(argc <= 2) Menu(); // Not processing a command
  
  rv = 1;
  while(rv) {
    if (!GXSTD::cin) {
      ClearInputStream(GXSTD::cin);
      if (!GXSTD::cin) {
	GXSTD::cout << "Input stream error" << "\n";
	return 0;
      }
    }
    GXSTD::cout << "\n";
    GXSTD::cout << '>';
    GXSTD::cin >> key;

    if (!GXSTD::cin) continue;
    switch(key) {
      case 'e' : case 'E' : 
	ClearInputStream(GXSTD::cin);
	InputData("Program Name", pname);
	break;
      case 'g' : case 'G' : 
	ClearInputStream(GXSTD::cin);
	InputData("Program Name", pname);
	InputData("User Name", user_name);
	srn.GenRegString(pname, user_name);
	PublishCode(pname.c_str(), user_name.c_str(), srn.GetRegCode());
	break;
      case 'u' : case 'U' :
	ClearInputStream(GXSTD::cin);
	if(pname.length() == 0) {
	  GXSTD::cout << "You must enter a program name using the (e) option" 
		      << "\n";
	  break;
	}
	InputData("User Name", user_name);
	srn.GenRegString(pname, user_name);
	PublishCode(pname.c_str(), user_name.c_str(), srn.GetRegCode());
	break;
      case 'v' : case 'V' : {
	ClearInputStream(GXSTD::cin);
	InputData("Program Name", pname);
	InputData("User Name", user_name);
	InputData("Registration Code", reg_code);
	srnGenerator ob1(srn);  // Test the copy constructor
	srnGenerator ob2 = ob1; // Test assignment operator
	i = ob2.Validate(reg_code, pname, user_name);
	GXSTD::cout << "\n";
	if(!i) GXSTD::cout << reg_code.c_str() << " is not valid" << "\n";
	else GXSTD::cout << "This code is valid" << "\n";
	GXSTD::cout << "\n";
	break;
      }
      case 'q' : case 'Q' :
	rv = Quit();
	break;
      case '?' :
	Menu();
	break;

      default:
        GXSTD::cout << "Unrecognized command" << "\n";
	GXSTD::cout << "\n";
    }
  }
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
