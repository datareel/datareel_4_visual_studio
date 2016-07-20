// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 08/09/2001
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

Console based test program used to test the gxRDBMS class. This test 
program demonstrates how to use the gxRDBMS class without definition 
files.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxrdbcon.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

int Quit()
{
  if(!gxrdRemovePID()) { 
    cout << "Error removing PID file" << "\n" << flush;
  }
  cout << "Exiting..." << "\n" << flush;
  return 0;
}

void Menu()
// Console based user menu.
{
  cout << "(A, a)    Add record to the database" << "\n" << flush;
  cout << "(b)       Batch insert test" << "\n" << flush;
  cout << "(B)       Batch delete test" << "\n" << flush;
  cout << "(c)       Change or add a database record" << "\n" << flush;
  cout << "(C)       Change existing database record" << "\n" << flush;
  cout << "(D, d)    Delete record from the database" << "\n" << flush;
  cout << "(F, f)    Find item by name" << "\n" << flush;          
  cout << "(L, l)    Dump database without using index file" << "\n" << flush; 
  cout << "(H, h, ?) Help (prints this menu)" << "\n" << flush;
  cout << "(I, i)    Dump database using the indexing subsystem" << "\n" 
       << flush;
  cout << "(Q, q)    Quit" << "\n" << flush;
  cout << "(r)       Rebuild index" << "\n" << flush;
  cout << "(R)       Rebuild data file" << "\n" << flush;
  cout << "(s)       Display index statistics" << "\n" << flush;
  cout << "(S)       Display data file statistics" << "\n" << flush;
  cout << "(t)       Check index" << "\n" << flush;
  cout << "(T)       Check data file" << "\n" << flush;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  int enable_auto_db_testing = 1;
  int enable_auto_recovery = 1;

  gxString table_name;
  gxDatabaseError err;
  gxRDBMS rdb;

  // Database definitions to read from the DEF file
  gxString table_def, key_def, family_def, fkey_def;
  
  // User and group definitions to read from the DEF file
  gxString user_def, group_def;
  
  // Process name and logging information
  gxString error_message, process_name;
  process_name = "testprog";

  // Set the table and key definition strings.
  table_def = "testdb,Item Name{25}VCHAR[15],Stock Number{12}INT32,Price{5}FLOAT.2"; 
  key_def = "PRI: COL 1,ATTRIB: NOCASE";

  // Set the table name
  table_name = table_def;
  table_name.DeleteAfterIncluding(",");
   
  if(enable_auto_db_testing) {
    // Test the table and key definitions then run consistency checks
    err = rdb.GenFileNames(table_def, key_def, family_def, fkey_def);
    if(err != gxDBASE_NO_ERROR) { // Test for any errors
      cout << "Bad table or key definition specified" << "\n" << flush;
      cout << gxDatabaseExceptionMessage(err) << "\n" << flush;
      cout << "Cannot open " << table_name << " database" << "\n" << flush;
      return 1;
    }
  }
  else {
    // Open or create the table without running consistency checks
    err = rdb.CreateTable(table_def, key_def, family_def, fkey_def);
    if(err != gxDBASE_NO_ERROR) { // Test for any errors
      cout << "Cannot open " << table_name << " database" << "\n" << flush;
      cout << gxDatabaseExceptionMessage(err) << "\n" << flush;
      return 1;
    }
  }

  if(enable_auto_db_testing) {
    if(gxrdHasPIDFile()) {
      cout << "\n" << flush;
      cout << "This application was not properly shutdown." << "\n" << flush;
      cout << "Performing emergency database consistency checks..." 
	   << "\n" << flush;
      if(!DatabaseConsistencyCheck(&rdb, enable_auto_recovery, 1)) {
	cout << "Emergency database consistency checks failed" 
	     << "\n" << flush;
	cout << "Cannot start application" << "\n" << flush;
	return 1;
      }
      cout << "\n" << flush;
    }
    else {
      if(!gxrdWritePID()) {
	cout << "Error writing PID file" << "\n" << flush;
      }
      cout << "\n" << flush;
      cout << "Performing routine database consistency checks" 
	   << "\n" << flush;
      if(!DatabaseConsistencyCheck(&rdb, enable_auto_recovery, 0)) {
	cout << "Routine database consistency checks failed" << "\n" << flush;
	cout << "Cannot start application" << "\n" << flush;
	return 1;
      }
      cout << "\n" << flush;
    }
  }

  char c;
  int rv = 1;
  Menu();
  
  while(rv) {
    if (!cin) { 
      ClearInputStream(cin);
      if (!cin) { 
	cout << "Input stream error" << "\n" << flush;
	return 0;
      }
    }
    cout << '>';
    cin >> c;
    if (!cin) continue;
    switch(c) {
      case 'a' : case 'A' : ClearInputStream(cin); AddRecord(&rdb); break;
      case 'b' : BatchInsert(&rdb); break;
      case 'B' : BatchDelete(&rdb); break;
      case 'c' : ClearInputStream(cin); ChangeOrAdd(&rdb); break;
      case 'C' : ClearInputStream(cin); ChangeRecord(&rdb); break;
      case 'd' : case 'D' : ClearInputStream(cin); DeleteRecord(&rdb); break;
      case 'f' : case 'F' : ClearInputStream(cin); FindRecord(&rdb); break;
      case 'l' : case 'L' : DumpDatabase(&rdb); break;	 
      case 'i' : case 'I' : DumpDatabase(&rdb, 0); break;
      case 'h' : case 'H' : case '?' : Menu(); break;
      case 'q' : case 'Q' : rv = Quit(); break;
      case 'r' : ClearInputStream(cin); RebuildIndex(&rdb); break;
      case 'R' : RebuildDataFile(&rdb); break;
      case 's' : ClearInputStream(cin); PrintBtreeHeader(&rdb); break;
      case 'S' : DatabaseStats(rdb.GetDataFile()); break;
      case 't' : ClearInputStream(cin); CheckIndex(&rdb); break;
      case 'T' : CheckDataFile(&rdb); break;
      default:
        cout << "Unrecognized command" << "\n" << flush;
    }
  }

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
