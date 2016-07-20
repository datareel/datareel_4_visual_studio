// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: gxbtree.cpp
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

Dictionary example program using the gxBtree cache functions.
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

#include <string.h>
#include <time.h>
#include "gxbtree.h"
#include "dfileb.h"
#include "ustring.h"
#include "leaktest.h"

const BtreeNodeOrder_t MyKeyClassOrder = 100;
const __WORD__ MyKeyNameSize = 54;
const int num_buckets = 3000;

class MyKeyClass : public DatabaseKeyB
{
public:
  MyKeyClass();
  MyKeyClass(const char *name);
  void operator=(const char *name);
  ~MyKeyClass() { }

public: // Base class interface
  size_t KeySize() { return sizeof(key_name); }
#ifndef __USE_SINGLE_COMPARE__
  int operator==(const DatabaseKeyB& key) const;
  int operator>(const DatabaseKeyB& key) const;
#else
  int CompareKey(const DatabaseKeyB& key) const;
#endif

public: // Persistent data member
  char key_name[MyKeyNameSize];
};

MyKeyClass::MyKeyClass() : DatabaseKeyB((char *)key_name)
{
  for(int i = 0; i < MyKeyNameSize; i++) key_name[i] = 0;
}

MyKeyClass::MyKeyClass(const char *name) : DatabaseKeyB((char *)key_name)
{
  strncpy(key_name, name,  MyKeyNameSize);
  key_name[ MyKeyNameSize-1] = 0; // Ensure null termination
}

void MyKeyClass::operator=(const char *name)
{
  strncpy(key_name, name,  MyKeyNameSize);
  key_name[ MyKeyNameSize-1] = 0; // Ensure null termination
}

#ifndef __USE_SINGLE_COMPARE__
int MyKeyClass::operator==(const DatabaseKeyB& key) const
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return (strcmp(key_name, (char *)kptr->db_key) == 0);
}

int MyKeyClass::operator>(const DatabaseKeyB& key) const
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return (strcmp(key_name, (char *)kptr->db_key) > 0);
}
#else
int MyKeyClass::CompareKey(const DatabaseKeyB& key) const
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return strcmp(key_name, (char *)kptr->db_key);
}
#endif //  __USE_SINGLE_COMPARE__

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
  PausePrg();
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
  PausePrg();
}

int ParseString(char *srt, char words[255][255],
		int*numwords, char sepchar)
// General purpose string parser
{
  int i = 0;
  char newword[255];
  *numwords = 0;

  // First skip over leading blanks. Stop if an ASCII NULL is seen
  while (1) {
    if (*srt == '\0') return 0;
    if (*srt != ' ') break;
    srt++;
  }
  
  while(1) {
    // Check to see if there is room for another word in the array
    if(*numwords == 255) return 1;

    i = 0;
    while (i < 254) {
      if(*srt == 0 || *srt == sepchar) break;
      newword[i] = *srt;
      srt++;
      i++;
    }
    newword[i] = 0;   // Ensure an ASCII null at end of newword. 
    strcpy (words[*numwords], newword);  // Install into array 
    (*numwords)++;
    
    // If stopped by an ASCII NULL above, exit loop
    if(*srt == 0) break;
    
    srt++;
    if(*srt == 0) break;
  }
  return 0;
}

int ImportTextFile(gxBtree &btx, char *fname)
{
  cout << "\n";
  cout << "Importing a dictionary file." << "\n";
  cout << "NOTE: Words must be delimited by a space or forward slash (/)"
       << "\n";
  
  int status, num, key_count = 0;
  char words[255][255];
  const int MaxLine = 255;
  char LineBuffer[MaxLine];
  const char dchar = '/';  // Text delimiter

  cout << "\n";
  cout << "Opening dictionary text file" << "\n";
  
  DiskFileB infile(fname);
  if(!infile) { // Could not open the istream
    cout << "Could not open file: " << fname << "\n";
    return 1;
  }
 
  cout << "Adding words..." << "\n";

  // Get CPU clock cycles before entering loop
  clock_t begin = clock();

  MyKeyClass key;
  MyKeyClass compare_key;
  BtreeCache btree_cache(num_buckets, &btx);
  if(!btree_cache) {
    cout << "Error constructing the B-tree cache object" << "\n";
    return 1;
  }

  while(!infile.df_EOF()) {
    if(infile.df_GetLine(LineBuffer, df_MAX_LINE_LENGTH, '\n', 1) != 
       DiskFileB::df_NO_ERROR) {
      break; // Error reading from the disk file
    }
    if(strcmp(LineBuffer, "") == 0) continue;
    
    if(ParseString(LineBuffer, words, &num, dchar) == 1) {
      return 1;
    }
    
    key = LineBuffer;

    // Cached insertion method
    status = btx.Insert(key, compare_key, &btree_cache);

    if (status != 1) {
      cout << "\n" << "Problem adding " << words[0] << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return 1;
    }
    else {
      key_count++;
    }
  }
  
  // Get CPU clock cycles after loop is completed 
  clock_t end =clock();
  
  // Calculate the elapsed time in seconds. 
  double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3); 
  cout << "Added " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);

  // Rewind the file
  infile.df_Rewind();
  
  cout << "Verifying the entries using a sort order search..." << "\n";
  begin = clock();
  key_count = 0;
  double search_time = 0;

  // Cache node used for optimized sort order searches
  BtreeCacheNode curr_node(btree_cache);

  while(!infile.df_EOF()) {
    if(infile.df_GetLine(LineBuffer, df_MAX_LINE_LENGTH, '\n', 1) != 
       DiskFileB::df_NO_ERROR) {
      break; // Error reading from the disk file
    }
    if(strcmp(LineBuffer, "") == 0) continue;
    
    if(ParseString(LineBuffer, words, &num, dchar) == 1) {
      return 1;
    }
    
    key = LineBuffer;
    clock_t begin_search = clock();

    // status = btx.Find(key, compare_key, &btree_cache);
    status = btx.Find(key, compare_key, curr_node);

    clock_t end_search = clock();
    search_time += (double)(end_search - begin_search) / CLOCKS_PER_SEC;

    if (status != 1) {
      cout << "\n" << "Problem finding " << words[0] << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return 1;
    }
    else {
      key_count++;
    }

  }

  end =clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3);
  cout << "Verified " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";
  double avg_search_time = (search_time/(double)key_count) * 1000;
  cout << "Average search time = " << avg_search_time << " milliseconds"
       << "\n";
  BtreeCacheStats(btree_cache);

  cout << "Verifying the entries using a find next sort order search..." 
       << "\n";
  begin = clock();
  key_count = 0;
  btx.ResetStats(); // Reset the B-tree tuning statistics

  if(btx.FindFirst(key, curr_node)) {
    key_count++;
    while(btx.FindNext(key, compare_key, curr_node)) {
      key_count++;
    }
  }
  else {
    cout << "\n" << "Problem finding first key" << "\n";
    cout << btx.DatabaseExceptionMessage() << "\n";
    return 1;
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
  btx.ResetStats(); // Reset the B-tree tuning statistics

  if(btx.FindLast(key, curr_node)) {
    key_count++;
    while(btx.FindPrev(key, compare_key, curr_node)) {
      key_count++;
    }
  }
  else {
    cout << "\n" << "Problem finding last key" << "\n";
    cout << btx.DatabaseExceptionMessage() << "\n";
    return 1;
  }
  
  end =clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout.precision(3);
  cout << "Verified " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);
  
  // Rewind the file
  infile.df_Rewind();

  cout << "Deleting the entries..." << "\n";
  begin = clock();
  key_count = 0;

  while(!infile.df_EOF()) {
    if(infile.df_GetLine(LineBuffer, df_MAX_LINE_LENGTH, '\n', 1) != 
       DiskFileB::df_NO_ERROR) {
      break; // Error reading from the disk file
    }
    if(strcmp(LineBuffer, "") == 0) continue;

    key = LineBuffer;

    // Balanced delete function
    // status = btx.Delete(key, compare_key, &btree_cache);

    status = btx.FastDelete(key, compare_key, &btree_cache);

    if (status != 1) {
      cout << "\n" << "Problem deleting " << words[0] << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return 1;
    }
    else {
      key_count++;
    }
  }

  end =clock();
  cout.precision(3);
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout << "Deleted " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);

  // Rewind the file
  infile.df_Rewind();

  cout << "Re-inserting all the words..." << "\n";
  key_count = 0;
  begin = clock();

  while(!infile.df_EOF()) {
    if(infile.df_GetLine(LineBuffer, df_MAX_LINE_LENGTH, '\n', 1) != 
       DiskFileB::df_NO_ERROR) {
      break; // Error reading from the disk file
    }
    if(strcmp(LineBuffer, "") == 0) continue;
    
    if(ParseString(LineBuffer, words, &num, dchar) == 1) {
      return 1;
    }
    
    key = LineBuffer;
    
    status = btx.Insert(key, compare_key, &btree_cache);

    if (status != 1) {
      cout << "\n" << "Problem adding " << words[0] << "\n";
      cout << btx.DatabaseExceptionMessage() << "\n";
      return 1;
    }
    else {
      key_count++;
    }
  }
  
  end =clock();
  cout.precision(3);
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  cout << "Added " << key_count << " words in " 
       << elapsed_time << " seconds" << "\n";

  BtreeCacheStats(btree_cache);

  cout << "Flushing the B-tree cache" << "\n";
  if(btree_cache.Flush() != gxDBASE_NO_ERROR) {
    cout << "Error flushing cache" << "\n";
    
  }
  cout << "Flushing the B-tree header" << "\n";
  btx.Flush();

  infile.df_Close();
  return 0;
}

int main(int argv, char **argc)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest(); 
#endif

  const char *fname = "testfile.btx"; // File name of this database
  char *dfile = "amerdict.txt"; // Default dictionary file
  char rev_letter = gxDatabaseRevisionLetter; // Set the default rev letter
  if(argv >= 2) { // Set a specified revision letter
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
    // to the version set by the gxDatabaseRevisionLetter constant.  
  }

  if(argv == 3) dfile = argc[2]; // Use specified dictionary

  MyKeyClass key, kbuf;
  gxBtree btx(key, MyKeyClassOrder);

  // Create a new B-tree index with one tree
  btx.Create(fname, rev_letter);

  ImportTextFile(btx, dfile);

  cout << "Exiting..." << "\n";
  return 0;
}

#endif // __USE_BTREE_CACHE__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
