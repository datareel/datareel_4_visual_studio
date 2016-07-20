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
 
Mutex example used to show how corruption can result if no
serialization is done.
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

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Constants
const int LOOPCONSTANT = 100000;
const int NUM_THREADS = 10;

// Mutex that will be shared by threads within this process 
gxMutex mutex;

// Global variables
int i, j, k, l;
int uselock = 1; // TRUE if locking shared data

class SimpleThread : public gxThread
{
public:
  SimpleThread() { }
  ~SimpleThread() { }
  
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

void *SimpleThread::ThreadEntryRoutine(gxThread_t *thread)
{
  for(int loop=0; loop < LOOPCONSTANT; ++loop) {
    if(uselock) { // Lock critical section if uselock is true
      if(mutex.MutexLock() != 0) {
	cout << mutex.MutexExceptionMessage() << "\n" << flush;
	return ExitThread(thread, 1);
      }
    }

    // Shared data
    ++i; ++j; ++k; ++l;

    if(uselock) {
      if(mutex.MutexUnlock() != 0) {
	cout << mutex.MutexExceptionMessage() << "\n" << flush;
	return ExitThread(thread, 1);
      }
    }
  }

  return 0;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  SimpleThread t;
  gxThread_t *thread[NUM_THREADS];
  int loop;
  
  cout << "Creating " << NUM_THREADS << " threads" << "\n" << flush;
  for(loop = 0; loop < NUM_THREADS; ++loop)
    thread[loop] = t.CreateThread();

  cout << "Wait for results" << "\n" << flush;
  t.sSleep(10);

  cout << "Show results" << "\n" << flush;
  cout << "Using " << NUM_THREADS << " threads and " << LOOPCONSTANT
       << " loop constant" << "\n" << flush;
  cout << "Values are: (should be " << (NUM_THREADS * LOOPCONSTANT)
       << ")" << "\n" << flush;
  cout << "  ==>" << i << ' ' << j << ' ' << k << ' ' << l << "\n" << flush;
                             
  cout << "Main thread completed" << "\n" << flush;

  // Prevent memory leaks
  for(loop = 0; loop < NUM_THREADS; ++loop) delete thread[loop];

  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

 
