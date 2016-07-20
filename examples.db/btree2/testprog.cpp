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

Dictionary example program using the gxBtree class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

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

const BtreeNodeOrder_t MyKeyClassOrder = 27;
const __WORD__ MyKeyNameSize = 26;

class MyKeyClass : public DatabaseKeyB
{
public:
  MyKeyClass();
  MyKeyClass(const char *name);
  void operator=(const char *name);
  ~MyKeyClass() { }

public: // Base class interface
  size_t KeySize() { return sizeof(key_name); }
  int operator==(const DatabaseKeyB& key) const;
  int operator>(const DatabaseKeyB& key) const;
  
  // NOTE: This comparison function is only used if the 
  // __USE_SINGLE_COMPARE__ preprocessor directive is 
  // defined when the program is compiled.
  int CompareKey(const DatabaseKeyB& key) const;

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

int MyKeyClass::CompareKey(const DatabaseKeyB& key) const
// NOTE: This comparison function is only used if the 
// __USE_SINGLE_COMPARE__ preprocessor directive is 
// defined when the program is compiled.
{
  const MyKeyClass *kptr = (const MyKeyClass *)(&key);
  return strcmp(key_name, (char *)kptr->db_key);
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
  intbuf << clear << (FAU_t)btx.Root();
  cout << "Root address =      " << intbuf.c_str() << "\n";
  cout << "Number of trees =   " << btx.NumTrees() << "\n";
  cout << "Number of entries = " << btx.NumKeys() << "\n";
  cout << "Number of nodes =   " << btx.NumNodes() << "\n";
  cout << "B-Tree order =      " << btx.NodeOrder() << "\n";
  cout << "B-Tree height =     " << btx.BtreeHeight() << "\n";
  PausePrg();
}

int ParseString(char *str, char words[255][255],
		int*numwords, char sepchar)
// General purpose string parser
{
  int i = 0;
  char newword[255];
  *numwords = 0;

  // First skip over leading blanks. Stop if an ASCII NULL is seen
  while (1) {
    if (*str == '\0') return 0;
    if (*str != ' ') break;
    str++;
  }
  
  while(1) {
    // Check to see if there is room for another word in the array
    if(*numwords == 255) return 1;

    i = 0;
    while(i < 254) {
      if(*str == 0 || *str == sepchar) break;
      newword[i] = *str;
      str++;
      i++;
    }
    newword[i] = 0;   // Ensure an ASCII null at end of newword. 
    strcpy (words[*numwords], newword);  // Install into array 
    (*numwords)++;
    
    // If stopped by an ASCII NULL above, exit loop
    if(*str == 0) break;
    
    str++;
    if(*str == 0) break;
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
  char LineBuffer[df_MAX_LINE_LENGTH];
  const char dchar = '/';  // Text delimiter

  cout << "\n";
  cout << "Enter the name of the text file to import" << "\n";
  
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
    status = btx.Insert(key, compare_key, 0);
    
    if (status != 1) {
      cout << "\n" << "Problem adding " << words[0] << "\n";
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
  BtreeStatus(btx);

  // Rewind the file
  infile.df_Rewind();
  
  cout << "Verifying the entries..." << "\n";
  begin = clock();
  key_count = 0;
  double search_time = 0;

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
    status = btx.Find(key, compare_key, 0);
    clock_t end_search = clock();
    search_time += (double)(end_search - begin_search) / CLOCKS_PER_SEC;

    if (status != 1) {
      cout << "\n" << "Problem finding " << words[0] << "\n";
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
  PausePrg();
  
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

    if(ParseString(LineBuffer, words, &num, dchar) == 1) {
      return 1;
    }
    
    if(strcmp(LineBuffer, "") == 0) continue;

    key = LineBuffer;
    status = btx.Delete(key, compare_key, 0);
    // status = btx.LazyDelete(key, compare_key, 0);

    if (status != 1) {
      cout << "\n" << "Problem deleting " << words[0] << "\n";
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
  BtreeStatus(btx);

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
    status = btx.Insert(key, compare_key, 0);
    
    if (status != 1) {
      cout << "\n" << "Problem adding " << words[0] << "\n";
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
  BtreeStatus(btx);

  infile.df_Close();
  return 0;
}

int main(int argv, char **argc)
{
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
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
