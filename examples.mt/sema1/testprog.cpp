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

Simple test program demonstrating the basic operation of the
gxThread class using semaphores
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
#include "gxsema.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Global semaphores objects
gxSemaphore child_counter;
gxSemaphore parent_counter;

// Class derived from the abstract gxThread base class 
class MessageClass : public gxThread
{
public:
  MessageClass() { }
  ~MessageClass() { }
  
public:
  char *message;

private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

void *MessageClass::ThreadEntryRoutine(gxThread_t *thread)
{
  cout << message << "\n" << flush;

  // The up operation increments the semaphore
  parent_counter.SemaphorePost();
  child_counter.SemaphorePost();
  return 0;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  MessageClass t;

  char *message1 = "Test message 1";
  char *message2 = "Test message 2";

  parent_counter.SemaphoreWait(); 
  
  child_counter--; // Semaphore value now 0 
  child_counter--; // Semaphore value now -1
  
  // The child_counter now must be incremented 2 times 
  // for a thread blocked on it to be released 
  
  t.message = message1;
  gxThread_t *tptr1 = t.CreateThread();
  
  // The wait operation blocks the thread if the semaphore
  // has a value less than or equal to zero.
  parent_counter.SemaphoreWait();
  
  t.message = message2;
  gxThread_t *tptr2 = t.CreateThread();
  
  // Force the parent thread to block until both children have
  // printed their message and executed the following semaphore
  // up function.
  child_counter.SemaphoreWait();
  
  // Wait for the last thread to print its message before exiting
  t.sSleep(1);

  cout << "\n";

  // Prevent memory leaks
  delete tptr1;
  delete tptr2;
  
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


  

