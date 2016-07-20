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

Program used to test the record locking primitives.
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
#include "gxfloat.h"
#include "gxdstats.h"

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

void ClearInputStream(istream &s)
// Used to clear istream
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

void Menu()
{
  cout << "\n";
  cout << "(c, C)   Clear the record lock header" << "\n";
  cout << "(h, H)   Help (prints this menu)" << "\n";
  cout << "(p)      Toggle the record lock protect" << "\n";
  cout << "(P)      Reset the record lock protect" << "\n";   
  cout << "(q, Q)   Quit this program" << "\n";
  cout << "(r)      Read lock the record" << "\n";
  cout << "(R)      Release the read lock" << "\n";
  cout << "(s, S)   Display the record stats" << "\n";
  cout << "(w)      Write lock the record" << "\n";
  cout << "(W)      Release the write lock" << "\n";
  cout << "\n";
}

void DisplayLockValues(gxRecordLockHeader &rlh)
{
  cout << "Record Lock Protect:     " << (int)rlh.record_lock_protect << "\n";
  cout << "Record Read Lock Value:  " << (int)rlh.record_read_lock << "\n";
  cout << "Record Write Lock Value: " << (int)rlh.record_write_lock << "\n"; 
}

int main(int argv, char **argc)
{
  char rev_letter = 'D'; // Set the revision letter
  if(argv == 2) { // Set a specified revision letter
    rev_letter = *argc[1];
    if(rev_letter == '0') rev_letter = 0;
    // Valid persistent record lock rev letters are:
    // Rev 'C' or 'c'
    // Rev 'D' or 'd'
  }

  gxDatabase *f = new gxDatabase; 
  cout << "Creating new file..." << "\n";
  const char *fname = "testfile.gxd";
  const int num_to_add = 500;
  
  if(!gxDatabase::Exists(fname)) {
    cout << "Creating new file..." << "\n";
    f->Create(fname, (FAU_t)0, rev_letter);
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

  FAU_t object_address = f->GetHighestBlock() + f->BlockHeaderSize();
  FAU_t block_address = f->GetHighestBlock();
  FAU_t header_address = f->GetHighestBlock() + sizeof(gxBlockHeader);
  
  char key;
  gxUINT32 lock_protect;
  Menu();
  int rv = 1;
  gxRecordLockHeader rh;

  while(rv) {
    if (!cin) { 
      ClearInputStream(cin); 
      if (!cin) {  
	cout << "Input stream error" << "\n";
	return 0;
      }
    }
    cout << '>';
    cin >> key;
    if (!cin) continue; 
    switch(key) {
      case '?' : Menu(); break;
      case 'c' : case 'C' : f->ResetRecordLock(block_address); break;
      case 'h' : case 'H' : Menu(); break;
      case 'q' : case 'Q' : rv = 0; break;
      case 's' : case 'S' : BlockStats(f, object_address); break;
      case 'p' :  
	lock_protect = 1;
	f->Write(&lock_protect, sizeof(gxUINT32), header_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      case 'P' :
	lock_protect = (gxUINT32)0;
	f->Write(&lock_protect, sizeof(gxUINT32), header_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      case 'r' : 
	f->LockRecord(gxDBASE_READLOCK, block_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      case 'R' : 
	f->UnlockRecord(gxDBASE_READLOCK, block_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      case 'w' : 
	f->LockRecord(gxDBASE_WRITELOCK, block_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      case 'W' : 
	f->UnlockRecord(gxDBASE_WRITELOCK, block_address);
	CheckError(f);
	f->ReadRecordLockHdr(rh, block_address);
	DisplayLockValues(rh);
	break;
      default:
        cout << "Unrecognized command" << "\n";
	break;
    }
  }

  cout << "Exiting..." << "\n";
  f->Close();
  if(CheckError(f) != 0) return 1;
  delete f;
  
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
