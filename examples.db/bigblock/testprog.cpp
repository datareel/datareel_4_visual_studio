// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp 
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 02/04/1997  
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

Program used to test the allocation of large database blocks.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxdbase.h"
#include "gxdstats.h"

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main()
{
  gxDatabase *f = new gxDatabase;
  if(!f) {
    cout << "Error allocating memory for database object" << "\n";
    return 1;
  }
  const char *fname = "bigblk.gxd";
  f->Create(fname);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }

  __UWORD__ xl_block = 10000000; // 10Meg bytes
  __UWORD__ el_block = 1000000;  // 1Meg bytes
  __UWORD__ vl_block = 100000;   // 100K bytes
  __UWORD__ l_block  = 10000;    // 10K bytes  
  
  cout << "Testing the allocation/removal of large blocks." << "\n";
  cout << "WARNING: This test will require 12 Meg of disk space." << "\n";
  cout << "Press Enter to continue or Ctrl-C to abort..." << "\n";
  cin.get();
  
  cout << "\n";
  cout << "Allocating " << xl_block << " bytes..." << "\n";
  FAU_t addr = f->Alloc(xl_block);
  f->CalcChecksum(xl_block, addr);
  cout << "Removing the block..." << "\n";
  f->Remove(addr);
  PausePrg();

  DatabaseStats(f);
  PausePrg();
  cout << "Allocating " << el_block << " bytes..." << "\n";
  addr = f->Alloc(el_block);
  f->CalcChecksum(el_block, addr);
  cout << "Removing the block..." << "\n";
  f->Remove(addr);
  PausePrg();

  DatabaseStats(f);
  PausePrg();
  cout << "Allocating " << vl_block << " bytes..." << "\n";
  addr = f->Alloc(vl_block);
  f->CalcChecksum(vl_block, addr);
  cout << "Removing the block..." << "\n";
  f->Remove(addr);
  PausePrg();

  DatabaseStats(f);
  PausePrg();
  cout << "Allocating " << l_block << " bytes..." << "\n";
  addr = f->Alloc(l_block);
  f->CalcChecksum(l_block, addr);
  cout << "Removing the block..." << "\n";
  f->Remove(addr);
  PausePrg();

  DatabaseStats(f);
  PausePrg();
  
  cout << "Exiting..." << "\n";
  f->Close();
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }

  delete f;
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
