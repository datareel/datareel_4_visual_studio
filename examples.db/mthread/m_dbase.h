// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: grocery.h
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997
// Date Last Modified: 01/29/2009
// Copyright (c) 2001-2009 DataReel Software Development
// ----------------------------------------------------------- // 
// ---------- Include File Description and Details  ---------- // 
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

This is a test program used to test the Persistent base class.
*/
// ----------------------------------------------------------- //   
#ifndef __M_DATABASE_HPP__
#define __M_DATABASE_HPP__

// DataReel includes
#include "gxdlcode.h"
#include "gxthread.h"
#include "gxmutex.h"
#include "gxcond.h"
#include "gpersist.h"

// My Database includes
#include "grocery.h" // Our persistent data type

// Constants used for my database application
const int DATABASE_THREAD_RETRIES = 255;

class MyDatabase
{
public:
  MyDatabase() { clear_all(); } 
  ~MyDatabase() { }

private: // Do not allow copying and assignment
  // NOTE: If you allow copying and/or assignment
  // NOTE: you must account for all open DB files.
  // NOTE: For this demo let's just force pointer
  // NOTE: symantics if we need to pass or store
  // NOTE: MyDatabase objects.
  MyDatabase(const MyDatabase &ob) { }
  void operator=(const MyDatabase &ob) { }

public: // Database access functions
  int Open(gxDatabaseAccessMode mode = gxDBASE_READWRITE,
	   char rev_letter = gxDatabaseRevisionLetter);
  int Add(Grocery &ob, int find_object = 1);
  int Change(Grocery &ob, Grocery &changedob, int find_object = 1);
  int Find(Grocery &ob, int read_object = 1);
  int Delete(Grocery &ob, int find_object = 1);

public: // Helper fucntions
  POD *GetDB() { return &DB; }
  gxDatabaseError LastError()
;
  const char *LastErrorMessage();
  int IsOpen();
  int Connect(Grocery &ob);
  int IsConnected(POD *poddb);
  void EnableFileSharing() { enable_smb_nfs = 1; }
  void DisableFileSharing() { enable_smb_nfs = 0; }
  void EnableLocking() { enable_processing_locking = 1; }
  void DisableLocking() { enable_processing_locking = 0; }

private: // Internal processing fucntions
  void clear_all();
  int application_database_lock();
  int application_database_unlock();

private:
  // Database objects
  POD DB;      // This is our database

  // True when procssing locking is enabled
  int enable_processing_locking;

  // True when database files are on shared file system
  int enable_smb_nfs; 

  // Our last recorded database error 
  gxDatabaseError last_error;

  // Application locking primitives
  gxMutex database_lock;     // Mutex used to lock the database
  gxCondition database_cond; // Condition variable used with the database lock
  int database_is_locked;    // Database lock boolean predicate
};

#endif // __M_DATABASE_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
