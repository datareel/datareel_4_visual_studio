// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
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

Program used to test the GXS library service file functions.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "gxsutils.h"
#include "dfileb.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int main(int argc, char **argv)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif
  
  // Check the command line arguments
  if(argc != 2) {
    cout << "\n";
    cout << "Usage: " << argv[0] << " services file" << "\n";
#if defined (__WIN32__)
    cout << "Example: " << argv[0] << " c:\\windows\\services" << "\n";
    cout << "Example: " << argv[0] 
	 << " c:\\winnt\\system32\\drivers\\etc\\services" << "\n";
    cout << "Example: " << argv[0] 
	 << " c:\\windows\\system32\\drivers\\etc\\services" << "\n";
#else
    cout << "Example: " << argv[0] << " /etc/services" << "\n";
#endif
    return 1;
  }

  char *fname = argv[1];
  char entry[df_MAX_LINE_LENGTH];
  
  DiskFileB infile(fname);
  if(!infile) {
    cout << "Cannot open the " << fname << " file" << "\n";
    return 1;
  }
        
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
  infile.df_Close();

  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
