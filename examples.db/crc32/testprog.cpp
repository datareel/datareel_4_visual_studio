// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 08/17/1998 
// Date Last Modified: 01/01/2009
// Copyright (c) 2001-2009 DataReel Software Development
// ----------------------------------------------------------- // 
// ------------- Program description and details ------------- // 
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
  
The CRC32 functions (Cyclic Redundancy Check) are used to
calculate a sophisticated checksum based on the algebra of
polynomials. The Cyclic Redundancy Check, is a way to detect
bit errors that occur during data storage or transmission.
The CRC-32 algorithm operates on a block of data as a single
large numerical value. The algorithm divides this large value
by the CRC-32 polynomial or generator polynomial, leaving the
remainder 32-bit, which is the checksum. 
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
#include "gxcrc32.h"

char in_file[255]; // Input file 

void HelpMessage(const char *program_name)
{
  cout << "\n";
  cout << "CRC-32 program." << "\n";
  cerr << "Displays 32-bit CRC for a specified file or string." << "\n";
  cout << "Usage: " << program_name << " [switches] infile" << "\n";
  cout << "Switches:  -?      = Display this help message." << "\n";
  cout << "           -sData  = Display 32-bit CRC for a specified string."
       << "\n";
  cout << "           -t      = Write a CRC-32 table to the standard output."
       << "\n";
  cout << "\n";
  exit(0);
}

void ProcessArgs(int argc, char *argv[])
// Process the program's argument list
{
  char *sbuf;
  int i;
  for(i = 1; i < argc; i++ ) {
    if(*argv[i] == '-') {
      char sw = *(argv[i] +1);
      switch(sw) {
	case '?' :
	  HelpMessage(argv[0]);
	  break;

	case 's': {
	  sbuf = &argv[i][2]; 
	  unsigned long crc;
	  crc = calcCRC32(sbuf, strlen(sbuf));
	  cout << sbuf << "\n";
	  cout << "CRC-32 = ";
	  cout.setf(ios::uppercase);
	  cout << "0x" << setfill('0') << setw(8) << hex << crc << "\n";
	  cout.unsetf(ios::uppercase);
	  exit(0);
	  break;
	}
	
	case 't':
	  makeCRC32(cout);
	  exit(0);
	  break;

	default:
	  cerr << "\n";
	  cerr << "Unknown switch " << argv[i] << "\n";
	  cerr << "Exiting..." << "\n";
	  cerr << "\n";
	  exit(0);
	  break;
      }
    }
    else { 
      strcpy(in_file, argv[i]); 
    }
  }
}

int main(int argc,char *argv[])
{
  // If no argument is given print usage message to the screen 
  if(argc < 2) {
    HelpMessage(argv[0]);
    return 0;
  }

  // Process the programs command line arguments
  ProcessArgs(argc, argv);

  if(in_file[0] == 0 ) {
    cout << "\n";
    cout << "You must specify a valid input file name." << "\n";
    cout << "\n";
    return 0;
  }

  DiskFileB infile(in_file);
  if(!infile) {
    cerr << "\n";
    cerr << "Cannot open file: " << in_file << "\n";
    cerr << "Exiting..." << "\n";
    cerr << "\n";
    return 1;
  }

  unsigned long crc = calcCRC32(infile);
  cout << in_file << "\n";
  cout << "CRC-32 = ";
  cout.setf(ios::uppercase);
  cout << "0x" << setfill('0') << setw(8) << hex << crc << "\n";
  cout.unsetf(ios::uppercase);
  infile.df_Close();
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
