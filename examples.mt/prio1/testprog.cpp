// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 03/25/2000
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
 
Test program demonstrating how to change thread priorities.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <stdlib.h>
#include "gxthread.h"
#include "gxcond.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// For safe condition variable usage, must use a Boolean predicate and 
// a mutex with the condition.
int is_locked = 1; // Block the threads initially in this example
gxCondition cond;
gxMutex mutex;

void PrintThreadStatus(gxThread_t *thread)
{
  cout << "Thread OID: " << (int)thread->GetObjectID() << ", "
       << thread->ThreadPriorityMessage() << ", " 
       << thread->ThreadPriorityClassMessage() << "\n" << flush;
}

void HandleThreadError(gxThread_t *thread)
{
  cout << "Error setting thread " << thread->GetObjectID()
       << " priority" << "\n" << flush;
  cout << thread->ThreadExceptionMessage() << "\n" << flush;
  cout << "Setting priority back to normal" << "\n" << flush;
  thread->SetThreadPriority(gxTHREAD_PRIORITY_NORMAL);
  thread->SetThreadPriorityClass(gxTHREAD_PRIORITY_CLASS_OTHER);
}

class SimpleThread : public gxThread
{
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

void *SimpleThread::ThreadEntryRoutine(gxThread_t *thread)
{
  int num = *((int *)thread->GetThreadParm());

  // Serialize access to the shared resource.
  mutex.MutexLock();
  
  int num_try = 0;
  while(is_locked != 0) {
    if(++num_try < 255) {
      cout << "Thread " << num << " execution blocked..." << "\n" << flush;
      cond.ConditionWait(&mutex);
    }
    else {
      cout << "Thread " << num << " could not wait for condition" 
	   << "\n" << flush;
      mutex.MutexUnlock();
      return 0;
    }
  }

  // Tell other threads to wait until shared resource is available
  is_locked = 1;

  // ********** Enter Critical Section ******************* //
  PrintThreadStatus(thread);
  // ********** Leave Critical Section ******************* //

  // Tell other threads shared resource is available
  is_locked = 0;

  // Wake up the next thread waiting on this condition
  cond.ConditionSignal();
  mutex.MutexUnlock();
  
  return 0;
}

// Main thread of execution
int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  SimpleThread t;

  const int NUM_THREADS = 10;
  gxThread_t *thread[NUM_THREADS];

  int i, rv;
  
  // Create the threads in a blocked state
  for(i = 0; i < NUM_THREADS; i++) {
    thread[i] = t.CreateThread((void *)&i);
    thread[i]->SetObjectID(i); // Assign an arbitrary object ID
    
    // Check for any errors created during thread creation
    if(thread[i]->GetThreadError() != gxTHREAD_NO_ERROR)
      cout << thread[i]->ThreadExceptionMessage() << "\n" << flush;
  }

  // Change some of the thread priorities and policies before executing them
  rv = t.SetThreadPriority(thread[NUM_THREADS-1], gxTHREAD_PRIORITY_HIGH);
  if(rv != 0) HandleThreadError(thread[NUM_THREADS-1]); 
 
  rv = t.SetThreadPriority(thread[NUM_THREADS-2], gxTHREAD_PRIORITY_HIGH, 
			   gxTHREAD_PRIORITY_CLASS_FIFO);
  if(rv != 0) HandleThreadError(thread[NUM_THREADS-2]); 

  rv = t.SetThreadPriority(thread[NUM_THREADS-3], gxTHREAD_PRIORITY_HIGH, 
			   gxTHREAD_PRIORITY_CLASS_RR);
  if(rv != 0) HandleThreadError(thread[NUM_THREADS-3]); 

  rv = t.SetThreadPriority(thread[0], gxTHREAD_PRIORITY_LOW);
  if(rv != 0) HandleThreadError(thread[0]); 
 
  rv = t.SetThreadPriority(thread[1], gxTHREAD_PRIORITY_LOW, 
			   gxTHREAD_PRIORITY_CLASS_FIFO);
  if(rv != 0) HandleThreadError(thread[1]); 

  rv = t.SetThreadPriority(thread[2], gxTHREAD_PRIORITY_LOW, 
			   gxTHREAD_PRIORITY_CLASS_RR);
  if(rv != 0) HandleThreadError(thread[2]); 

  // Reset lock and wake up all the waiters
  is_locked = 0;
  cond.ConditionBroadcast();
  
  // Wait for all the threads to finish before exiting
  for(i = 0; i < NUM_THREADS; i++) t.JoinThread(thread[i]);

  // Cleanup all the gxThread_t pointers
  for(i = 0; i < NUM_THREADS; i++) {
    rv = t.DestroyThread(thread[i]);
    if(rv != 0) {
      cout << "Error destroying thread!" << "\n" << flush;
    }
  }

  return 0; // Exit the process
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


  

