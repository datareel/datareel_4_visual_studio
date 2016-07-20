// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 08/22/2000 
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

gxBtree cache test program used to test the insertion and deletion 
time of a large number of keys.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if !defined (__USE_BTREE_CACHE__)
int main() { return 0; }
#else

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <time.h>
#include "gxdstats.h"
#include "gxbtree.h"
#include "ustring.h"

// Set the Btree order
// const BtreeNodeOrder_t MyKeyClassOrder = 8;
// const BtreeNodeOrder_t MyKeyClassOrder = 16;
// const BtreeNodeOrder_t MyKeyClassOrder = 32;
// const BtreeNodeOrder_t MyKeyClassOrder = 64;
const BtreeNodeOrder_t MyKeyClassOrder = 128;
// const BtreeNodeOrder_t MyKeyClassOrder = 256;
// const BtreeNodeOrder_t MyKeyClassOrder = 1024;

// Set the Btree cache size
const int num_buckets = 1000;

class MyKeyClass : public DatabaseKeyB
{
public:
  MyKeyClass() : DatabaseKeyB((char *)&data) { }
  MyKeyClass(gxUINT32 i) : DatabaseKeyB((char *)&data) { data = i; }

public: // Base class interface
  size_t KeySize() { return sizeof(data); }
  int operator==(const DatabaseKeyB& key) const;
  int operator>(const DatabaseKeyB& key) const;

  // NOTE: This comparison function is only used if the 
  // __USE_SINGLE_COMPARE__ preprocessor directive is 
  // defined when the program is compiled.
  int CompareKey(const DatabaseKeyB& key) const;
  
public: // Persistent data member
  gxUINT32 data;
};

int MyKeyClass::operator==(const DatabaseKeyB& key) const
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return data == *((gxUINT32 *)kptr->db_key);
}

int MyKeyClass::operator>(const DatabaseKeyB& key) const
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return data > *((gxUINT32 *)kptr->db_key);
}

int MyKeyClass::CompareKey(const DatabaseKeyB& key) const
// NOTE: This comparison function is only used if the 
// __USE_SINGLE_COMPARE__ preprocessor directive is 
// defined when the program is compiled.
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  if(data == *((gxUINT32 *)kptr->db_key)) return 0;
  if(data > *((gxUINT32 *)kptr->db_key)) return 1;
  return -1;
}

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

void BtreeStatus(gxBtree &btx)
{
  UString intbuf;
  cout << "\n";
  intbuf << clear << btx.Root();
  cout << "Root address =      " << intbuf.c_str() << "\n";
  cout << "Number of trees =   " << btx.NumTrees() << "\n";
  cout << "Number of entries = " << btx.NumKeys() << "\n";
  cout << "Number of nodes =   " << btx.NumNodes() << "\n";
  cout << "B-tree order =      " << btx.NodeOrder() << "\n";
  cout << "B-tree height =     " << btx.BtreeHeight() << "\n";
  cout << "\n";
}

void BtreeCacheStats(BtreeCache &btree_cache)
{
  cout << "\n";
  cout << "--------- Cache tuning statistics ---------" << "\n";
  cout << "Number of dirty buckets = " 
       << btree_cache.BucketsInUse() << "\n";
  cout << "Number of disk reads    = " 
       << btree_cache.uncached_reads << "\n";
  cout << "Number of cached reads  = " 
       << btree_cache.cached_reads << "\n";
  cout << "\n";
}

void BuildTree(gxBtree &btx)
{
  // Adjust this number to set the number of insertions
  const unsigned long INSERTIONS = 1 * 1000;       // 1K test
  // const unsigned long INSERTIONS = 10 * 1000;      // 10K test
  // const unsigned long INSERTIONS = 100 * 1000;     // 100K test
  // const unsigned long INSERTIONS = 1000 * 1000;    // 1MEG test
  // const unsigned long INSERTIONS = 10000 * 1000;   // 10MEG test
  // const unsigned long INSERTIONS = 100000 * 1000;  // 100MEG test
  // const unsigned long INSERTIONS = 1000000 * 1000; // 1GIG test
  // const unsigned long INSERTIONS = 2000000 * 1000; // 2GIG test

  MyKeyClass key;
  MyKeyClass compare_key;

  cout << "Inserting " << INSERTIONS << " keys..." << "\n";
  const int echo_results = 1;
  unsigned long i, key_count = 0;
  unsigned long curr_count = 0;
  int rv;
  int verify_deletions = 0; // Set to true to verify all deletions
  double insert_time = 0;
  
  BtreeCache btree_cache(num_buckets, &btx);
  if(!btree_cache) {
    cout << "Error constructing the B-tree cache object" << "\n";
    return;
  }

  // Get CPU clock cycles before entering loop
  clock_t begin = clock();

  for(i = 0; i < INSERTIONS; i++) {
    key.data = i;
    clock_t begin_insert = clock();

    // Cached insertion method
    rv = btx.Insert(key, compare_key, &btree_cache);

    clock_t end_insert = clock();
    insert_time += (double)(end_insert - begin_insert) / CLOCKS_PER_SEC;
    key_count++;
    curr_count++;

    if(rv != 1) {
      cout << "\n" << "Problem adding key - " << i << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return;
    }
    if(curr_count == 10000) {
      // Optimize the cache by flushing the LRU buckets
      btree_cache.LRUFlush(1000);
      if(echo_results == 1) {
	curr_count = 0;
	cout << "Inserted " << i << " keys in " << insert_time
	     << " seconds" << "\n";
      }
    }
  }
  
  // Get CPU clock cycles after loop is completed 
  clock_t end =clock();

  // Calculate the elapsed time in seconds. 
  double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3); 
  cout << "Inserted " << key_count << " values in " 
       << elapsed_time << " seconds" << "\n";
  double avg_insert_time = (insert_time/(double)key_count) * 1000;
  cout << "Average insert time = " << avg_insert_time << " milliseconds"  
       << "\n"; 

  BtreeCacheStats(btree_cache);

  key_count = 0;
  double search_time = 0;
  cout << "Verifying the insertions..." << "\n";

  // Cached node used for optimized sort order searches
  BtreeCacheNode curr_node(btree_cache);

  begin = clock();
  for(i = 0; i < INSERTIONS; i++) {
    key.data = i;
    clock_t begin_search = clock();

    // Optimized sort order search.
    rv = btx.Find(key, compare_key, curr_node);

    clock_t end_search = clock();
    key_count++;
    search_time += (double)(end_search - begin_search) / CLOCKS_PER_SEC;
    
    if(rv != 1) {
      cout << "Error finding key - " << i << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return;
    }
  }

  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3);
  cout << "Verified " << key_count << " values in " 
       << elapsed_time << " seconds" << "\n";
  double avg_search_time = (search_time/(double)key_count) * 1000;
  cout << "Average search time = " << avg_search_time << " milliseconds"
       << "\n";

  BtreeCacheStats(btree_cache);

  cout << "Verifying the entries using a find next sort order search..." 
       << "\n";
  begin = clock();
  key_count = 0;

  // Walk through the tree starting at the first node
  if(btx.FindFirst(key, curr_node)) {
    key_count++;
    while(btx.FindNext(key, compare_key, curr_node)) {
      key_count++;
    }
  }
  else {
    cout << "\n" << "Problem finding first key" << "\n";
    cout << btx.DatabaseExceptionMessage() << "\n";
    return;
  }
  
  end =clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3);
  cout << "Verified " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);

  cout << "Verifying the entries using a find prev sort order search..." 
       << "\n";
  begin = clock();
  key_count = 0;

  // Walk through the tree starting at the last node
  if(btx.FindLast(key, curr_node)) {
    key_count++;
    while(btx.FindPrev(key, compare_key, curr_node)) {
      key_count++;
    }
  }
  else {
    cout << "\n" << "Problem finding last key" << "\n";
    cout << btx.DatabaseExceptionMessage() << "\n";
    return;
  }
  
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3);
  cout << "Verified " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);

  cout << "Deleting all the entries..." << "\n";
  key_count = 0;
  double delete_time = 0;
  begin = clock();
  for(i = 0; i < INSERTIONS; i++) {
    key.data = i;
    clock_t begin_delete = clock();

    rv = btx.Delete(key, compare_key, &btree_cache);
    // rv = btx.FastDelete(key, compare_key, &btree_cache);

    clock_t end_delete = clock();
    delete_time += (double)(end_delete - begin_delete) / CLOCKS_PER_SEC;
    key_count++;
    if(rv != 1) {
      cout << "Error deleting key - " << i << "\n";
      return;
    }

    if(verify_deletions) { // Verify the remaining key locations
      for(unsigned long j = INSERTIONS-1; j != i; j--) {
	key.data = j;
	rv = btx.Find(key, compare_key, 0);
	if(rv != 1) {
	  cout << "Error finding key  - " << j << "\n";
	  cout << "After deleting key - " << i << "\n";
	  cout << btx.DatabaseExceptionMessage() << "\n";
	  return;
	}
      }
    }
  }

  end =clock();
  cout.precision(3);
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout << "Deleted " << key_count << " values in " 
       << elapsed_time << " seconds" << "\n";
  double avg_delete_time = (delete_time/(double)key_count) * 1000;
  cout << "Average delete time = " << avg_delete_time << " milliseconds"
       << "\n";

  cout << "\n";
  cout << "Re-inserting " << INSERTIONS << " keys..." << "\n";
  key_count = 0;
  insert_time = 0;
  begin = clock();
  for(i = 0; i < INSERTIONS; i++) {
    key.data = i;
    clock_t begin_insert = clock();

    rv = btx.Insert(key, compare_key, &btree_cache);

    clock_t end_insert = clock();
    insert_time += (double)(end_insert - begin_insert) / CLOCKS_PER_SEC;
    key_count++;
    curr_count++;

    if(curr_count == 10000) {
      // Optimize the cache by flushing the LRU buckets
      btree_cache.LRUFlush(1000);
    }

    if(rv != 1) {
      cout << "\n" << "Problem adding key - " << i << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return;
    }
  }
  end =clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3); 
  cout << "Inserted " << key_count << " values in " 
       << elapsed_time << " seconds" << "\n";
  avg_insert_time = (insert_time/(double)key_count) * 1000;
  cout << "Average insert time = " << avg_insert_time << " milliseconds"  
       << "\n"; 
}

int main(int argv, char **argc)
{
  const char *fname = "testfile.btx"; // File name of this database
  char rev_letter = gxDatabaseRevisionLetter; // Set the default rev letter
  if(argv == 2) { // Set a specified revision letter
    rev_letter = *argc[1];
    if(rev_letter == '0') rev_letter = '\0';
    // Valid rev letters are:
    // Rev 0
    // Rev 'A' or 'a'
    // Rev 'B' or 'b'
    // Rev 'C' or 'c'
    // Rev 'D' or 'd'
    // Rev 'E' or 'e'
    // NOTE: The gxDatabase class will set invalid revision letters
    // the version set by the gxDatabaseRevisionLetter constant.  
  }

  MyKeyClass key, compare_key;
  gxBtree btx(key, MyKeyClassOrder);

  // Create a new Btree index file
  btx.Create(fname, rev_letter);
  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
  
  // Build the Btree
  BuildTree(btx);
  
  cout << "\n";
  cout << "Exiting..." << "\n";
  return 0;
}

#endif // __USE_BTREE_CACHE__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
