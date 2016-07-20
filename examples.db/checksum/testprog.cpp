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

gxDatabase persistent checksum example.
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

const int name_length = 16;

class DatabaseObject
{ 
public:
  DatabaseObject() { name[0] = 0; oid = (gxINT32)0, cid = (gxFLOAT64)0; }
  DatabaseObject(const char *s, long i, double d);

public:
  void DisplayObject();
  
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

void DatabaseObject::DisplayObject()
{
  cout << "Database object name: " << name << "\n";
  cout << "Database object OID:  " << oid << "\n";
  cout.setf(ios::showpoint | ios::fixed);
  cout.precision(3);
  cout << "Database object CID:  " << cid << "\n";
}

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main(int argv, char **argc)
{
  char rev_letter = 'D'; // Set the revision letter
  if(argv == 2) { // Set a specified revision letter
    rev_letter = *argc[1];
    if(rev_letter == '0') rev_letter = 0;
    // Valid persistent record lock rev letters are:
    // Rev 'A' or 'a'
    // Rev 'B' or 'b'
    // Rev 'C' or 'c'
    // Rev 'D' or 'd'
  }

  gxDatabase *f = new gxDatabase;
  if(!f) {
    cout << "Error allocating memory for database file pointer..." << "\n";
    return 1;
  }
  const char *fname = "simple.gxd";
  int exists;

  cout << "\n";
  cout << "Simple checksum example." << "\n";

  if(!gxDatabase::Exists(fname)) {
    cout << "Creating new file..." << "\n";
    f->Create(fname, (FAU_t)0, rev_letter);
    if(CheckError(f) != 0) {
      delete f;
      return 1;
    }
    exists = 0;
  }
  else {
    cout << "Opening existing file..." << "\n";
    f->Open(fname);
    if(CheckError(f) != 0) {
      delete f;
      return 1;
    }
    exists = 1;
  }
  
  PausePrg();
  DatabaseStats(f);
  PausePrg();
  
  FAU addr;
  int rv;
  __ULWORD__ object_crc, calc_crc;
  
  if(!exists) {
    cout << "Adding object to database file..." << "\n";
    // Allocate a block to store the object
    addr = f->Alloc(sizeof(DatabaseObject)); 

    // Construct the datbase object
    DatabaseObject ob("Mouse", (long)addr, 1035.101);

    f->Write(&ob, sizeof(DatabaseObject));
    
    f->WriteObjectChecksum(addr);
    f->ReadObjectChecksum(addr, &object_crc, &calc_crc);
    cout.setf(ios::uppercase);
    cout << "Object CRC =     0x" << setfill('0') << setw(8) << hex
	 << object_crc << "\n";
    cout << "Calculated CRC = 0x" << setfill('0') << setw(8) << hex
	 << calc_crc << dec << "\n";
    BlockStats(f, addr);
  }
  else {
    f->Open(fname); 
    cout << "Reading the first object from the database file..." << "\n";
    addr = f->FindFirstBlock();

    // CRC checksum routines
    rv = f->ReadObjectChecksum(addr+f->BlockHeaderSize(),
			       &object_crc, &calc_crc);
    if(!rv) cout << "Checksum error!" << "\n";
    cout.setf(ios::uppercase);
    cout << "Object CRC =     0x" << setfill('0') << setw(8) << hex
	 << object_crc << "\n";
    cout << "Calculated CRC = 0x" << setfill('0') << setw(8) << hex
	 << calc_crc << dec << "\n";
    cout.unsetf(ios::uppercase);
    cout << "\n";

    DatabaseObject buf;
    f->Read(&buf, sizeof(DatabaseObject), addr+f->BlockHeaderSize());
    buf.DisplayObject();
  }

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
