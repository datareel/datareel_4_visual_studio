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

Program used to test the file locking primitives.
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
  cout << "(c, C)   Clear the file lock header" << "\n";
  cout << "(h, H)   Help (prints this menu)" << "\n";
  cout << "(p)      Toggle the file lock protect" << "\n";
  cout << "(P)      Reset the file lock protect" << "\n";   
  cout << "(q, Q)   Quit this program" << "\n";
  cout << "(r)      Read lock the file" << "\n";
  cout << "(R)      Release the read lock" << "\n";
  cout << "(s, S)   Display the file stats" << "\n";
  cout << "(w)      Write lock the file" << "\n";
  cout << "(W)      Release the write lock" << "\n";
  cout << "\n";
}

void DisplayLockValues(gxFileLockHeader &flh)
{
  cout << "File Lock Protect:     " << (int)flh.file_lock_protect << "\n";
  cout << "File Read Lock Value:  " << (int)flh.file_read_lock << "\n";
  cout << "File Write Lock Value: " << (int)flh.file_write_lock << "\n"; 
}

int main(int argv, char **argc)
{
  char rev_letter = 'E'; // Set the revision letter
  if(argv == 2) { // Set a specified revision letter
    rev_letter = *argc[1];
    if(rev_letter == '0') rev_letter = 0;
    // Valid persistent file lock rev letters are:
    // Rev 'B' or 'b'
    // Rev 'C' or 'c'
    // Rev 'D' or 'd'
    // Rev 'E' or 'e'
  }

  gxDatabase *f = new gxDatabase; 
  cout << "Creating new file..." << "\n";
  const char *fname = "testfile.gxd";

  if(!gxDatabase::Exists(fname)) {
    cout << "Creating new file..." << "\n";
    f->Create(fname, (FAU_t)0, rev_letter);
    if(CheckError(f) != 0) {
      delete f;
      return 1;
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
  
  char key;
  Menu();
  int rv = 1;
  gxFileLockHeader lh;
  gxUINT32 lock_protect;

  while(rv) {
    if (!cin) { 
      ClearInputStream(cin); 
      if (!cin) {  
	cout << "Input stream error" << "\n";
	delete f;
	return 0;
      }
    }
    cout << '>';
    cin >> key;
    if (!cin) continue;
    switch(key) {
      case '?' : Menu(); break;
      case 'c' : case 'C' : f->ResetFileLock(); break;
      case 'h' : case 'H' : Menu(); break;
      case 'q' : case 'Q' : rv = 0; break;
      case 's' : case 'S' : DatabaseStats(f); break;
      case 'p' : 
	lock_protect = 1;
	f->Write(&lock_protect, sizeof(gxUINT32), sizeof(gxFileHeader));
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      case 'P' : 
	lock_protect = (gxUINT32)0;
	f->Write(&lock_protect, sizeof(gxUINT32), sizeof(gxFileHeader));
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      case 'r' : 
	f->LockFile(gxDBASE_READLOCK);
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      case 'R' : 
	f->UnlockFile(gxDBASE_READLOCK);
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      case 'w' : 
	f->LockFile(gxDBASE_WRITELOCK);
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      case 'W' : 
	f->UnlockFile(gxDBASE_WRITELOCK);
	CheckError(f);
	f->ReadFileLockHdr(lh);
	DisplayLockValues(lh);
	break;
      default:
        cout << "Unrecognized command" << "\n";
	break;
    }
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
