// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/20/1999
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

This is a simple test program for the device cache class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
#include <fstream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#include <fstream.h>
#endif // __USE_ANSI_CPP__

#include "devcache.h"
#include "memblock.h"
#include "membuf.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Example class used to create an iostream cache
class devCache : public gxDeviceCache
{
public:
  devCache();
  ~devCache() { }

private: // Base class interface
  void Read(void *buf, unsigned bytes, gxDeviceTypes dev);
  void Write(const void *buf, unsigned bytes, gxDeviceTypes dev);
};

devCache::devCache()
{ 
  ready_for_writing = 1; 
  ready_for_reading = 1;
}

void devCache::Read(void *buf, unsigned bytes, gxDeviceTypes dev) 
// Derived class buffered read function.
{
  switch(dev) {
    case gxDEVICE_CONSOLE:
      if(!cin) {
	ready_for_reading = 0;
	return;
      }
      else {
	ready_for_reading = 1;
      }
      cin.getline((char *)buf, bytes);
      break;

    default:
      break;
  }
}
  
void devCache::Write(const void *buf, unsigned bytes, gxDeviceTypes dev) 
// Derived class buffered write function.
{
  switch(dev) {
    case gxDEVICE_CONSOLE:
      if(!cout) {
	ready_for_writing = 0;
	return;
      }
      else {
	ready_for_writing = 1;
      }
      // Write the exact number of bytes allocated during a read operation
      // cout.write((char *)buf, bytes);

      // In this example "buf" is a null-terminated string
      cout << (char *)buf << "\n"; 
      break;
      
    default:
      break;
  }
}

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  devCache device;
  unsigned cache_size = 10;
  gxDeviceBucketCache cache(cache_size);

  if(!cache) {
    cout << "Could not allocate memory for the cache" << "\n";
    return 1;
  }
  
  cout << "Creating a device cache using " << cache_size 
       << " cache buckets." << "\n";
  cout << "Bucket size = " << sizeof(gxDeviceBucket) << "\n";
  cout << "Reserving " << (sizeof(gxDeviceBucket) * cache_size) 
       << " bytes of memory." << "\n";
  cout << "\n";
  
  cache.Connect(&device);

  // Setup a pointer to the cache buckets
  gxDeviceTypes o_device = gxDEVICE_CONSOLE; // Buffered output device
  gxDeviceTypes i_device = gxDEVICE_CONSOLE; // Buffered input device
  gxDeviceCachePtr p1(cache, o_device, i_device);

  cout << "Performing a buffered console read" << "\n";
  const int num_reads = 5;
  for(int i = 1; i < (num_reads+1); i++) {
    // Prompt the user before performing a buffered console read
    cout << "Enter string #" << i << ": ";

    p1.Alloc(255); // Allocate and read cache bucket
    if(!p1) {
      cout << "Error reserving a device cache bucket" << "\n";
      return 1;
    }
  }
    
  cout << "\n";
  cout << "Buckets in use = " << cache.BucketsInUse() << "\n";
  cout << "Flushing the device cache" << "\n";
  cache.Flush();
  cout << "Buckets in use = " << cache.BucketsInUse() << "\n";
  
  PausePrg();
  
  // Testing the device cache pointer's copy constructor
  // and overloaded assignment operator
  gxDeviceCachePtr p2(p1);
  gxDeviceCachePtr p3 = p1;
  
  // Construct and initialize some null terminated strings
  char *s1 = new char[4];
  char *s2 = new char[4];
  char *s3 = new char[4];
  char *s4 = new char[4];
  strcpy(s1, "CAT"); strcpy(s2, "COW"); strcpy(s3, "DOG"); strcpy(s4, "RAT");
  
  cout << "Testing the gxDeviceCachePtr overloaded arrow and indirection \
operator" << "\n";

  // Test the overloaded arrow operator
  p2->Load(s1, 4); // Allocate and load a bucket
  if(!p2) {
    cout << "Error reserving a device cache bucket" << "\n";
    return 1;
  }
  p2->Load(s2, 4); // Allocate and load new bucket
  p2->Load(s3, 4);

  // Test the overloaded indirection operator
  gxDeviceBucket *b = &(*p3);
  b->Load(s4, 4);

  cout << "Flushing the device cache using the bucket cache destructor \
call" << "\n";  

  // Each bucket stores a unique copy of these memory segments
  // so the memory allocated for the strings can safely be deleted
  // before the cache is flushed.
  delete[] s1;
  delete[] s2;
  delete[] s3;
  delete[] s4;

  return 0; // The bucket cache destructor call will flush the cache
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
