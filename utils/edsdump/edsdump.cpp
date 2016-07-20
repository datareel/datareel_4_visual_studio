// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: edsdump.cpp 
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 10/15/1999 
// Date Last Modified: 04/10/2009
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

Code used to dump the eds configuration files.

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
#else // Use the old iostream library by default
#include <iostream.h>
#include <iomanip.h>
#endif // __USE_ANSI_CPP__

#include "edscfg.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Version number and program name
const double edsDumpVersionNumber = edsVersionNumber;
const char *ProgramName = "edsdump";

// Program globals
unsigned num_files = 0;   // Total number of files processed

void HelpMessage(const char *program_name, const double version_number)
{
  GXSTD::cout << "\n";
  GXSTD::cout.setf(GXSTD::ios::showpoint|GXSTD::ios::fixed);
  GXSTD::cout.precision(3);
  GXSTD::cout << program_name << " program version " << version_number << "\n";
  GXSTD::cout << "Usage: " << program_name << " infile.eds" << "\n"; 
  GXSTD::cout << "\n";
}

int ProcessArgs(char *arg)
{
  switch(arg[1]) {

    default:
      GXSTD::cerr << "\n";
      GXSTD::cerr << "Unknown switch " << arg << "\n";
      GXSTD::cerr << "Exiting..." << "\n";
      GXSTD::cerr << "\n";
      return 0;
  }

  arg[0] = '\0';
  return 1; // All command line arguments were valid
}

void Dump(char *fname)
{
  edsConfig *CfgData = new edsConfig(fname);

  FAU_t oa;         // Object Address
  gxBlockHeader gx; // Block Header
  FAU_t gxdfileEOF = CfgData->OpenDatabase()->GetEOF();
  FAU_t addr = 0;
  // Search the entire file
  addr = CfgData->OpenDatabase()->FindFirstBlock(addr);
  edsWORD *eds_string;

  if(addr == (FAU_t)0) { // No database blocks found in file
    CfgData->Close();
    delete CfgData;
    return; 
  }
  
  while(1) {
    if(addr >= gxdfileEOF) break;
    CfgData->OpenDatabase()->Read(&gx, sizeof(gxBlockHeader), addr);
    if(gx.block_check_word == gxCheckWord) {
      if((__SBYTE__)gx.block_status == gxNormalBlock) {
	oa = addr + CfgData->OpenDatabase()->BlockHeaderSize();
	eds_string = CfgData->Read(oa);
	char str[edsMaxLine];
	unsigned i = 0;
	for(i = 0; i < edsMaxLine; i++) str[i] = 0; // Clear the string buffer
	for(i = 0; i < edsMaxLine; i++) {
	  unsigned char c;
	  if(eds_string[i] == edsEOT) break;
	  if(CfgData->DecodeString(eds_string[i], c)) str[i] = c;
	}
	UString buf(str);
	edsConfigNode eds_node(buf, addr);
	edsConfigListNode *chsptr = CfgData->Add(eds_node);
	delete eds_string;
	if(!chsptr) {
	  CfgData->Close();
	  delete CfgData;
	  return;
	}
      }
      addr = addr + gx.block_length; // Goto the next database block
    }
    else {
      addr = CfgData->OpenDatabase()->FindFirstBlock(addr); 
      if(!addr) break;
    }
  }

  edsConfigListNode *ptr = (edsConfigListNode *)CfgData->GetHead();
  while(ptr) {
    GXSTD::cout << ptr->node_data.str.c_str() << ' ' << "\n";
    ptr = ptr->GetNext();
  }
  CfgData->Close();
  delete CfgData;
}

// Program's main thread of execution.
// ----------------------------------------------------------- 
int main(int argc, char *argv[]) // Array of command-line argument 
// NOTE: None of the MSVC compilers will expand wildcard characters
// used in command-line arguments unless linked with the setargv.obj
// library. All the UNIX compliers will expand wildcard characters
// by default.
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  // If no arguments are given print usage message to the screen 
  if(argc < 2) {
    HelpMessage(ProgramName, edsDumpVersionNumber);
    return 0;
  }

  // Process command ling arguments and files 
  int narg;
  char *arg = argv[narg = 1];
  while (narg < argc) {
    if (arg[0] != '\0') {
      if (arg[0] == '-') { // Look for command line arguments
	if(!ProcessArgs(arg)) return 0; // Exit if argument is not valid
      }
      else { 
	if(gxDatabase::Exists(arg)) {
	  Dump(arg);
	  num_files++;
	}
      }
      arg = argv[++narg];
    }
  }
    
  if(num_files == 0) {
    GXSTD::cerr << "\n";
    GXSTD::cerr << "You must enter a file name." << "\n";
    GXSTD::cerr << "Exiting..." << "\n";
    GXSTD::cerr << "\n";
    return 0;
  }
  
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
