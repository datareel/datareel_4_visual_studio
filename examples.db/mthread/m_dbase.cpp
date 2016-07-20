// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Source Code File Name: grocery.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997
// Date Last Modified: 01/29/2009
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

This is a test program used to test the Persistent base class.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#include "m_dbase.h"

int MyDatabase::Open(gxDatabaseAccessMode mode, char rev_letter)
{
  if(IsOpen()) return 1;

  // NOTE: These variables can be class memebers or passed as 
  // NOTE: function arguments if you want to make them configurable 
  // NOTE: for each database opened. In this example a single database 
  // NOTE: is used so these values will be the same for each database.
  // NOTE: The below values are presented as named variables for 
  // NOTE: demo purposes only.
  BtreeNodeOrder_t order = POD_DEFAULT_ORDER;
  int num_trees = 1; // Use one tree per index file
  int use_index = 1;
  const char *data_file = "grocery.gxd";
  const char *index_file = "grocery.btx";
  GroceryKey key_type;
  __SBYTE__ df_rev_letter = rev_letter;
  __SBYTE__ if_rev_letter = rev_letter;
  FAU_t static_size = (FAU_t)65535; 
  static_size *= 2; // Set aside some static storage space in the DB file
  last_error = DB.Open(data_file, index_file, key_type,
			order, mode, use_index, 
			static_size, num_trees, df_rev_letter, 
			if_rev_letter);
  
  if(last_error != gxDBASE_NO_ERROR) {
    // TODO: Do any additional error processing here if needed 
    return 0;
  }

  return 1;
}

int MyDatabase::application_database_lock()
{
  if(!enable_processing_locking) {
    if(enable_smb_nfs) DB.TestDatabase();
    return 1;
  }

  database_lock.MutexLock();

  // Tell other threads to wait until process lock complete
  int num_try = 0;
  while(database_is_locked != 0) {

    // Block this thread from its own execution if a another
    // application thread is processing 
    if(++num_try < DATABASE_THREAD_RETRIES) {
      database_cond.ConditionWait(&database_lock);
    }
    else {
      last_error = gxDBASE_FILELOCK_ACCESS_ERROR;
      return 0; // Could not lock the database
    }
  }

  // Tell other threads to wait until process is complete
  database_is_locked = 1; 

  // ********** Entering Critical Section ********** // 

  last_error = gxDBASE_NO_ERROR;

  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access on a shared
  // file system.
  if(enable_smb_nfs) DB.TestDatabase();

  return 1;
}

int MyDatabase::application_database_unlock()
{
  if(!enable_processing_locking) {
    if(enable_smb_nfs) DB.Flush(); 
    return 1;
  }

  // Tell other threads that this process is complete
  database_is_locked = 0; 
 
  // Wake up the next thread waiting on this condition
  database_cond.ConditionSignal();
  database_lock.MutexUnlock();

  // Flushing the database buffers on shared file systems
  if(enable_smb_nfs) DB.Flush(); 

  // ********** Leaving Critical Section ********** //

  return 1;
}

int MyDatabase::Add(Grocery &ob, int find_object)
{
  if(!Connect(ob)) return 0;

  if(find_object) {
    if(Find(ob, 0)) {
      last_error = gxDBASE_OBJECT_EXISTS;
      DB.SetDataFileError(last_error);
      return 0;
    }
  }

  if(!application_database_lock())  return 0;  
  if(!ob.WriteObject()) {
    application_database_unlock();
    return 0;
  }

  if(!application_database_unlock()) return 0;

  return 1;
}

int MyDatabase::Change(Grocery &ob, Grocery &changedob, int find_object)
{
  if(!Connect(ob)) return 0;

  if(find_object) {
    if(!Find(ob, 1)) return 0;
  }

  if(!application_database_lock())  return 0;
  if(!ob.ChangeObject(changedob)) {
    application_database_unlock();
    return 0;
  }
  if(!application_database_unlock()) return 0;

  return 1;
}

int MyDatabase::Find(Grocery &ob, int read_object)
{
  if(!Connect(ob)) return 0;

  if(!application_database_lock())  return 0;

  if(!ob.FindObject()) {
    last_error = gxDBASE_OBJECT_NOT_FOUND;
    DB.SetDataFileError(last_error);
    application_database_unlock();
    return 0;
  }
  if(read_object) {
    if(!ob.ReadObject()) {
      application_database_unlock();
      return 0;
    }
  }
  
  if(!application_database_unlock()) return 0;

  return 1;
}

int MyDatabase::Delete(Grocery &ob, int find_object)
{
  if(!Connect(ob)) return 0;

  if(find_object) {
    if(!Find(ob, 0)) return 0;
  }

  if(!application_database_lock())  return 0;
  if(!ob.DeleteObject()) {
    application_database_unlock();
    return 0;
  }
  if(!application_database_unlock()) return 0;

  return 1;
}

gxDatabaseError MyDatabase::LastError() 
{ 
  last_error = DB.GetDataFileError();
  if(last_error == gxDBASE_NO_ERROR) {
    last_error = DB.GetIndexFileError();
  }
  return last_error; 
}

const char *MyDatabase::LastErrorMessage() 
{ 
  return gxDatabaseExceptionMessage(LastError());
}

int MyDatabase::Connect(Grocery &ob)
{
  if(!IsConnected(ob.GetDatabase())) {
    last_error = ob.ConnectDB(&DB);
    if(last_error != gxDBASE_NO_ERROR) {
      DB.SetDataFileError(last_error);
      return 0;
    }
  }
  return 1;
}

int MyDatabase::IsConnected(POD *poddb) 
{ 
  if(poddb != &DB) {
    return 0;
  }
   return 1; 
}

int MyDatabase::IsOpen() 
{ 
  if(!DB.OpenDataFile()) {
    return 0; 
  }
  return 1; 
}

void MyDatabase::clear_all() 
{
  last_error = gxDBASE_NO_ERROR;
  database_is_locked = 0;
  enable_smb_nfs = 1;
  enable_processing_locking = 1;
}

// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
