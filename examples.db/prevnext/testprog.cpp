// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 02/04/1997  
// Date Last Modified: 07/16/2009
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

gxDatabase linear navigation test.
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
#include "gxdbase.h"
#include "gxfloat.h"
#include "gxdstats.h"
#include "ustring.h"

const int name_length = 16;

class DatabaseObject
{ 
public:
  DatabaseObject() { name[0] = 0; oid = (gxINT32)0, cid = (gxFLOAT64)0; }
  DatabaseObject(const char *s, long i, double d);

public:
  friend void NewNode(char *s, int i, double d, gxDatabase *f);
  
public: // Platform independent data members
  char name[name_length]; // Fixed string type
  gxINT32 oid;            // Integer type
  gxFLOAT64 cid;          // Floating point type
};

DatabaseObject::DatabaseObject(const char *s, long i, double d)
{
  for(int j = 0; j < name_length; j++) name[j] = 0; // Clear the name string
  strcpy(name, s);
  oid = i;  
  cid = d;
}

void NewNode(char *s, int i, double d, gxDatabase *f) 
{
  DatabaseObject dbo(s, i, d);
  f->Write(&dbo, sizeof(dbo), f->Alloc(sizeof(dbo)));
}

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main()
// Simple demo of how to use the file manager class.
{
  gxDatabase *f = new gxDatabase;
  const char *fname = "prevnext.gxd";
  UString intbuf;

  cout << "\n";
  cout << "Simple navigation demo." << "\n";
  cout << "Testing gx find functions..." << "\n";
  const int num_to_add = 5;

  if(!gxDatabase::Exists(fname)) {
    cout << "Creating new file..." << "\n";
    f->Create(fname);
    if(CheckError(f) != 0) {
      delete f;
      return 1;
    }
    cout << "Adding " << num_to_add << " objects..." << "\n";
    for(long i = 0; i < num_to_add; i++) {
      NewNode("Mouse", i, 2000.101, f);
      if(CheckError(f) != 0) {
	delete f;
	return 1;
      }
    }
  }
  else {
    cout << "Opening existing file..." << "\n";
    f->Open(fname);
    if(CheckError(f) != 0) {
      delete f;
      return 1;
    }
  }

  PausePrg();
  DatabaseStats(f);
  PausePrg();

  cout << "Walking through the file finding all blocks..." << "\n";
  FAU_t block_address = 0;
  while(1) {
    block_address = f->FindFirstBlock(block_address);
    if(block_address == FAU_t(0)) break;
    intbuf << clear << block_address;
    cout << "Found block at file address: " << intbuf.c_str() << "\n";
    block_address++; // Offset the block address
  }

  PausePrg();
  
  cout << "Finding all blocks in reverse order..." << "\n";
  block_address = f->GetEOF();
  while(1) {
    block_address = f->FindPrevBlock(block_address);
    if(block_address == FAU_t(0)) break;
    intbuf << clear << block_address;
    cout << "Found block at file address: " << intbuf.c_str() << "\n";
    block_address++; // Offset the block address
  }

  PausePrg();

  cout << "Walking through the file finding every object..." << "\n";
  FAU_t object_address = 0;
  while(1) {
    object_address = f->FindFirstObject(object_address);
    if(object_address == FAU_t(0)) break;
    intbuf << clear << object_address;
    cout << "Found object at file address: " << intbuf.c_str() << "\n";
    object_address++; // Offset the block address
  }

  PausePrg();

  cout << "Finding every object in reverse order..." << "\n";
  object_address = f->GetEOF();
  while(1) {
    object_address = f->FindPrevObject(object_address);
    if(object_address == FAU_t(0)) break;
    intbuf << clear << object_address;
    cout << "Found object at file address: " << intbuf.c_str() << "\n";
    object_address++; // Offset the block address
  }
  
  cout << "\n";
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
