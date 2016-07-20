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
 
Simple test program demonstrating barrier synchronization
using the gxSemaphore class.
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

// Global semaphore objects
gxSemaphore semaA, semaB;

// Classes derived from the gxThread base class 
class Simple1 : public gxThread
{
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

class Simple2 : public gxThread
{
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

void *Simple1::ThreadEntryRoutine(gxThread_t *thread)
{
  cout << "Thread 1 here, before the barrier." << "\n";
  semaB.SemaphorePost();
  semaA.SemaphoreWait();
  cout << "Thread 1 after the barrier." << "\n";
  return 0;
}

void *Simple2::ThreadEntryRoutine(gxThread_t *thread)
{
  cout << "Thread 2 here, before the barrier." << "\n";
  semaA.SemaphorePost();
  semaB.SemaphoreWait();
  cout << "Thread 2 after the barrier." << "\n";
  return 0;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif
  
  Simple1 t1;
  Simple2 t2;

  gxThread_t *t1_thread = t1.CreateThread();
  gxThread_t *t2_thread = t2.CreateThread();

  t1.JoinThread(t1_thread);
  t2.JoinThread(t2_thread);

  // Prevent memory leaks
  delete t1_thread;
  delete t2_thread;

  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


  

