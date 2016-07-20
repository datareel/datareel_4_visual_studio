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
gxThread class using mutex and condition variable synchronization 
primitives.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxthread.h"
#include "gxmutex.h"
#include "gxcond.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Constants
const int max_reps = 10;

// Global variables
char buffer;
int buffer_has_item = 0;
int num_reps;

// Global synchronization objects
gxMutex mutex;
gxCondition condition;

// Standalone function that works with the reader class 
inline char make_new_item() { return 'A' + num_reps; }
inline void consume_item(char &c) { c = 0; }
void WriterFunction();

// Class derived from the abstract gxThread base class 
class ReaderClass : public gxThread
{
public:
  ReaderClass() { }
  ~ReaderClass() { }
  
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

// Program's main thread of execution
int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  // Set the rep count to zero
  num_reps = 0;
  
  ReaderClass t;
  gxThread_t *tptr = t.CreateThread();
  
  WriterFunction();
  
  // Wait for the thread to consume the last item before exiting
  t.sSleep(3);

  delete tptr; // Prevent memory leaks
  return 0;
}

void *ReaderClass::ThreadEntryRoutine(gxThread_t *thread)
{
  while(num_reps < max_reps) {
    mutex.MutexLock();
    condition.ConditionTimedWait(&mutex, 1);
    if(buffer_has_item == 1) {
      cout << "Comsuing item: " << buffer << "\n" << flush;
      consume_item(buffer);
      buffer_has_item = 0;
    } 
    mutex.MutexUnlock();
  } 
  return 0;
}
  
void WriterFunction()
{
  while(num_reps < max_reps) {
    mutex.MutexLock();
    condition.ConditionTimedWait(&mutex, 1);
      if(buffer_has_item == 0) {
	buffer = make_new_item();
	cout << "Making item: " << buffer << "\n" << flush;
	buffer_has_item = 1;
	num_reps++;
      }
      mutex.MutexUnlock();
  }
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


  

