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

Demo of how to re-open a database file using the gxDatabase
manager.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include "gxdbase.h"
#include "gxdstats.h"
#include "gxfloat.h"

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
{
  gxDatabase *f = new gxDatabase; 
  const char *fname1 = "cat.gxd";
  const char *fname2 = "dog.gxd";
  const char *fname3 = "mouse.gxd";
  
  cout << "\n";

  cout << "Testing the gxDatabase::ReOpen() function." << "\n";
  if(!gxDatabase::Exists(fname1)) f->Create(fname1);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }
  if(!gxDatabase::Exists(fname2)) f->Create(fname2);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }
  if(!gxDatabase::Exists(fname3)) f->Create(fname3);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }

  cout << "\n";
  cout << "Writing one object to: " << fname1 << "\n";
  f->Open(fname1);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }
  NewNode("Cat", 0L, 2000.101, f);

  PausePrg();
  DatabaseStats(f);
  PausePrg();

  cout << "Writing two objects to: " << fname2 << "\n";
  f->Open(fname2);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }
  NewNode("Cat", 0L, 2000.101, f);
  NewNode("Dog", 0L, 2000.101, f);
  PausePrg();
  DatabaseStats(f);
  PausePrg();

  cout << "Writing three objects to: " << fname3 << "\n";
  f->Open(fname3);
  if(CheckError(f) != 0) {
    delete f;
    return 1;
  }
  NewNode("Cat", 0L, 2000.101, f);
  NewNode("Dog", 0L, 2000.101, f);
  NewNode("Mouse", 0L, 2000.101, f);
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
