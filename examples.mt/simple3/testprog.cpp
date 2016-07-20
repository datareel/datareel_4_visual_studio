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
gxThread class using parameter passing.
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

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

class SimpleThread : public gxThread
{
private: // Base class interface
  void *ThreadEntryRoutine(gxThread_t *thread);
};

void *SimpleThread::ThreadEntryRoutine(gxThread_t *thread)
{
  // Cast the thread parameter to the correct type
  cout << (char *)thread->GetThreadParm() << "\n" << flush;
  return 0;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  SimpleThread t;
  char *message1 = "The quick brown fox jumps over the lazy dog";

  // Test the parameter passing to create function
  gxThread_t *tptr = t.CreateThread((void *)message1);

  // Wait for the thread to exit
  t.sSleep(1); 

  delete tptr; // Prevent memory leaks
  return 0; // Exit the process
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


  

