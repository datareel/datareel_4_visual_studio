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

Test program used to the basic functionality of the Thread Pool
class.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "thrpool.h"
#include "gthreadt.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void PausePrg()
// Pause the program and wait for the user to press enter.
{
  // Pause the program until enter is pressed
  cout << "\n" << flush;
  cout << "Press Enter to continue..." << "\n" << flush;
  cin.get();
}

void ScrollForward(thrPool &pool)
{
  cout << "Walking through the pool..." << "\n" << flush;
  thrPoolNode *ptr = pool.GetHead();
  while(ptr) {
    cout << (char)ptr->GetThreadPtr()->GetObjectID() << ' ';
    ptr = ptr->GetNext();
  } 
  cout << "\n" << flush;
}

void RewindPool(thrPool &pool)
{
  cout << "Walking through the pool in a backward direction..." 
       << "\n" << flush;
  thrPoolNode *ptr = pool.GetTail();

  // Walk through the pool in a backward direction
  while(ptr) {
    cout << (char)ptr->GetThreadPtr()->GetObjectID() << ' ';
    ptr = ptr->GetPrev();
  } 

  cout << "\n" << flush;
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  thrPool pool;
  thrPoolNode *ptr;

  cout << "Loading data elements into the pool..." << "\n" << flush;
  thrPoolNode *na = new thrPoolNode;
  thrPoolNode *nb = new thrPoolNode;
  thrPoolNode *nc = new thrPoolNode;
  thrPoolNode *nd = new thrPoolNode;
  thrPoolNode *n1 = new thrPoolNode;
  thrPoolNode *n2 = new thrPoolNode;
  thrPoolNode *n3 = new thrPoolNode;
  thrPoolNode *n4 = new thrPoolNode;

  // Allocating some memory buffers
  const unsigned num_blocks = 9;
  gxThread_t *buf[num_blocks];
  unsigned i;
  for(i = 0; i < num_blocks; i++) {
    buf[i] = new gxThread_t;
    // Set an arbitary object ID stating starting with ASCII character 'A'
    buf[i]->SetObjectID(65+i);
  }
  
  na->SetThreadPtr(buf[0]);
  nb->SetThreadPtr(buf[1]);
  nc->SetThreadPtr(buf[2]);
  nd->SetThreadPtr(buf[3]);

  // Load some pool elements
  pool.InsertAfter(pool.GetHead(), na);
  pool.InsertAfter(na, nb);
  pool.InsertAfter(nb, nc);
  pool.InsertAfter(nc, nd);
  
  n1->SetThreadPtr(buf[4]);
  n2->SetThreadPtr(buf[5]);
  n3->SetThreadPtr(buf[6]);
  n4->SetThreadPtr(buf[7]);
  
  // Testing insert functions
  pool.InsertBefore(na, n1);
  pool.InsertBefore(nc, n2);
  pool.InsertAfter(nb, n3);
  pool.InsertAfter(nd, n4);

  // Reordering the pool
  pool.MoveToBack(na);
  pool.MoveAfter(na, nc);
  pool.MoveBefore(nc, nb);
  pool.MoveAfter(nc, nd);
  pool.MoveAfter(nd, n2);
  pool.MoveBefore(n2, n1);
  pool.MoveAfter(n2, n3);
  pool.MoveAfter(n3, n4);
  
  ScrollForward(pool);
  RewindPool(pool);

  PausePrg();

  cout << "Adding another item to the pool..." << "\n" << flush;
  ptr = pool.AddThread(buf[8]);
  ScrollForward(pool);

  PausePrg();

  cout << "Moving pool item to the front of the pool" << "\n" << flush;
  pool.MoveToFront(ptr);
  ScrollForward(pool);

  cout << "Moving item to the back of the pool..." << "\n" << flush;
  pool.MoveToBack(ptr);
  ScrollForward(pool);
  
  PausePrg();
  
  cout << "Removing the head of the pool..." << "\n" << flush;
  ptr = pool.GetHead();
  gxThread_t *tptr = pool.RemoveNode(ptr);
  if(tptr) delete tptr; // Prevent memory leaks
  ScrollForward(pool);

  cout << "Removing the tail of the pool..." << "\n" << flush;
  ptr = pool.GetTail();
  tptr = pool.RemoveNode(ptr);
  if(tptr) delete tptr; // Prevent memory leaks
  ScrollForward(pool);

  PausePrg();
  cout << "Testing for memory leaks..." << "\n" << flush;
  unsigned len = sizeof(gxThread_t);
  const unsigned num_to_allocate = 1000; // 1000 * 1000;
  const unsigned mem_bufs = (sizeof(thrPoolNode) + len) * num_to_allocate;
  cout << "Allocating " << mem_bufs << " bytes..." << "\n" << flush;

  // Clear the thread pool without checking the status of each thread
  pool.ClearPool();

  unsigned count1 = 0;
  unsigned count2 = 0;
  for(i = 0; i < num_to_allocate; ++i) {
    gxThread_t *sbuf = new gxThread_t;
    count1 += sizeof(sbuf);
    if(!sbuf) {
      cout << "Memory allocation error at " << (count1+count2) << " bytes"
	   << "\n" << flush;
      break;
    }
    
    count2 += sizeof(thrPoolNode);
    if(!pool.AddThread(sbuf))  {
      cout << "Memory allocation error at " << (count1+count2) << " bytes"
	   << "\n" << flush;
      break;
    }
  }
  cout << "Done" << "\n" << flush;
  PausePrg();

  cout << "Releasing memory allocated back to the heap..." << "\n" << flush;

  while(!pool.IsEmpty()) {
    // Clear the pool, giving the application a chance to check the
    // status of each thread.
    gxThread_t *t = pool.RemoveTail();
    if(t) {
      // Process the thread and release any memory the thread is holding
      // ...
      delete t;
    }
  }
  pool.MakeEmpty(); // The thread pool is no empty
  
  PausePrg();
  
  cout << "Exiting..." << "\n" << flush;
  cout << "\n" << flush;
  return 0;
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //


