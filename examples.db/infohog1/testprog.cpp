// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997
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

Test program for the InfoHog database API.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "fstring.h"
#include "infohog.h"
#include "gxfloat.h"
#include "ustring.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Type definitions
typedef FString Name_t;        // String type
typedef gxFLOAT64 Price_t;     // Floating point type         
typedef gxINT32 StockNumber_t; // Integer type

void PausePrg()
// Function used to pause the program.
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

void ClearInputStream(istream &s)
// Function used to clear the input stream.
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

int Quit()
{
  cout << "Exiting..." << "\n";
  return 0;
}

void InputData(Name_t &key)
// Function used to read a string from the input stream.
{
  cout << "Item name: ";
  cin >> key;
}

void DisplayItem(InfoHog<Name_t> &infohog, int full = 1)
// Function used to print a infohog list object to the stdout.
{
  cout << "\n";
  if(infohog.GetMember(0)) {
    cout << "Item Name    = " << (char *)infohog.GetMember(0) << "\n";
  }
  if(full) {
    if(infohog.GetMember(1)) {
      cout << "Stock Number = " 
	   << *((StockNumber_t *)infohog.GetMember(1)) << "\n";
    }
    if(infohog.GetMember(2)) {
      cout.setf(ios::showpoint | ios::fixed);
      cout.precision(2);
      cout << "Price        = $" 
	   << *((Price_t *)infohog.GetMember(2)) << "\n";
    }
  }
}

void PrintBtreeHeader(POD *DB, unsigned index_number) 
{
  gxBtreeHeader hdr;
  UString intbuf;
  DB->Index(index_number)->ReadBtreeHeader(hdr);
  cout << "B-tree header information" << "\n";

  cout << "Node order:      " << hdr.node_order << "\n";
  cout << "Key size:        " << hdr.key_size << "\n";
  cout << "Number of nodes: " << hdr.n_nodes << "\n";
  cout << "B-tree height:   " << hdr.btree_height << "\n";
  intbuf << clear << (FAU_t)hdr.root;
  cout << "Root Address:    " << intbuf.c_str() << "\n";
  cout << "Number of trees: " << hdr.num_trees << "\n";
  cout << "\n";
}

void Menu()
// Console based user menu.
{
  cout << "(A, a)    Add object to the database" << "\n";
  cout << "(B, b)    Build a test database" << "\n";
  cout << "(C)       Change data members" << "\n";
  cout << "(c)       Change object" << "\n";
  cout << "(D, d)    Delete object from the database" << "\n";
  cout << "(F, f)    Find item by name" << "\n";          
  cout << "(L, l)    List without using index file" << "\n"; 
  cout << "(H, h, ?) Help (prints this menu)" << "\n";
  cout << "(I, i)    List using the index file" << "\n";
  cout << "(Q, q)    Quit" << "\n";
  cout << "(S, s)    Display B-tree stats" << "\n";
  cout << "(X, x)    Compare the index file to the data file" << "\n";
  cout << "(Y, y)    Rebuild the index file" << "\n";
}

void BuildDatabase(POD *DB)
// Function used to build a test database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  const int NUM_OBJECTS = 1000;
  int i = 0;
  InfoHog<Name_t> infohog(DB);

  char name[InfoHogNameLength];
  StockNumber_t stock_number = (StockNumber_t)2000;
  Price_t price = (Price_t)0.95;

  cout << "Adding " << NUM_OBJECTS << " objects to the database..." << "\n";
  for(i = 0; i < NUM_OBJECTS; i++) {
    sprintf(name, "Item-%i", i);
    stock_number += i;
    price += i;
    Name_t key(name);
    if(!infohog.SetMember(&key, sizeof(key), 0)) {
      cout << "Could not set data member 0" << "\n";
      return;
    }
    if(!infohog.SetMember(&stock_number, sizeof(stock_number), 1)) {
      cout << "Could not set data member 1" << "\n";
      return;
    }
    if(!infohog.SetMember(&price, sizeof(price), 2)) {
      cout << "Could not set data member 2" << "\n";
      return;
    }
    if(!infohog.WriteObject()) {
      cout << "Could not add object number " << i << " to the database"
	   << "\n";
      return;
    }
  }

  // Flush the database files after a batch insert
  DB->Flush();
}

void ListInOrder(POD *DB, unsigned index_number)
// List the contents of the database using the index file.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  InfoHog<Name_t> infohog(DB);
  InfoHogKey<Name_t>key, compare_key;
  gxBtree *btx = DB->Index(index_number);
 
  // Walk through the tree starting at the first key
  if(btx->FindFirst(key)) {
    if(!infohog.ReadObject(key.ObjectID())) {
      cout << "Error reading the object" << "\n";
      return;
    }
    DisplayItem(infohog);
    
    while(btx->FindNext(key, compare_key)) {
      if(!infohog.ReadObject(key.ObjectID())) {
	cout << "Error reading the object" << "\n";
	return;
      }
      DisplayItem(infohog);
    }
  }
}

void ListInOrder(POD *DB)
// List contents of the database without using the index file.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  FAU_t addr = (FAU_t)0;
  InfoHog<Name_t> infohog(DB);
  unsigned count = 0;
  ClearInputStream(cin); // Clear input stream
  while(1) {
    addr = DB->OpenDataFile()->FindFirstObject(addr);
    if(!addr) break;
    if(!infohog.ReadObject(addr)) {
      cout << "Error reading the object" << "\n";
      return;
    }
    DisplayItem(infohog);
    count++;
    if(count == 2) {
      PausePrg();
      count = 0;
    }
  }
}

void AddItem(POD *DB)
// Function used to add an object to the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  Name_t key;
  InputData(key);

  InfoHog<Name_t> infohog(DB);
  infohog.SetMember(&key, sizeof(key), 0);

  int exists = infohog.FindObject();

  if(exists) {
    cout << "Item: " << key << " already exists" << "\n";
    return;
  }

  int stock_number;
  double price;

  cout << "Enter the items's stock number: ";
  cin >> stock_number;
  if(cin) {
    StockNumber_t sn = stock_number; 
    infohog.SetMember(&sn, sizeof(StockNumber_t), 1);
    cout << "Enter the items's price:       $";
    cin >> price;
  } 
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  if(cin) {
    Price_t p = price;
    infohog.SetMember(&p, sizeof(Price_t), 2);
  }
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  
  ClearInputStream(cin);
  
  if(!infohog.WriteObject()) {
    cout << "Could not add object to the database" << "\n";
  }

  DB->Flush(); // Flushing the database buffers following manual insertion
}

void DeleteItem(POD *DB)
// Function used to delete an object from the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  Name_t key;
  InputData(key);

  InfoHog<Name_t> infohog(DB);
  infohog.SetMember(&key, sizeof(key), 0);
  if(!infohog.DeleteObject()) {
    cout << "Could not find item: " << key << " in the database" << "\n";
    return;
  }

  cout << "Deleted item: " << key << "\n";

  DB->Flush(); // Flushing the database buffers following manual deletion
}

void FindItem(POD *DB)
// Function used to find an object in the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  Name_t key;
  InputData(key);

  InfoHog<Name_t> infohog(DB);
  infohog.SetMember(&key, sizeof(key), 0);

  if(!infohog.FindObject()) {
    cout << "Could not find item: " << key << " in the database" 
	 << "\n";
    return;
  }

  cout << "Found item: " << key << "\n";
  
  if(!infohog.ReadObject()) {
    cout << "Error reading the object" << "\n";
    return;
  }
  DisplayItem(infohog); 
}

void Compare(POD *DB)
// Function used to compare the contents of the data file to the
// index file.
{
  if(!DB->UsingIndex()) {
    cout << "This database is not using the indexing sub-system" << "\n";
    return;
  }
  
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  InfoHog<Name_t> infohog(DB);
  cout << "\n";
  cout << "Comparing the index file to the data file..." << "\n";
  int rv = infohog.CompareIndex(0);
  if(!rv) {
    cout << "The index file does not match the data file!" << "\n";
    cout << "The index file needs to be rebuilt." << "\n";
    cout << "\n";
    return;
  }

  cout << "The index file checks good" << "\n";
  cout << "\n";
}

void Rebuild(POD *DB, const char *fname)
// Function used to rebuild the index file if the index file
// entries no longer match the data file entries.
{
  if(!DB->UsingIndex()) return;
  
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  InfoHog<Name_t> infohog(DB);
  cout << "\n";
  cout << "Rebuilding the index file..." << "\n";

  // Rebuild index number 0
  int rv = infohog.RebuildIndexFile(fname, 0, 
				    InfoHogNumTrees, InfoHogNodeOrder);
  if(!rv) {
    cout << "The index file was not rebuilt!" << "\n";
    cout << "\n";
    return;
  }

  cout << "The index file was rebuilt." << "\n";
  cout << "A new index file named " << fname << " was created.";
  cout << "\n";
}

void ChangeItem(POD *DB, int change_object = 1)
{
  Name_t key;
  InputData(key);
  
  InfoHog<Name_t> infohog(DB);
  infohog.SetMember(&key, sizeof(key), 0);
  
  if(!infohog.FindObject()) {
    cout << "Could not find item: " << key << " in the database" 
	 << "\n";
    return;
  }
  
  cout << "Found item: " << key << "\n";
  
  if(!infohog.ReadObject()) {
    cout << "Error reading the object" << "\n";
    return;
  }
  DisplayItem(infohog); 
  
  ClearInputStream(cin);
  cout << "Entering changes to this object..." << "\n";
  Name_t new_key;
  InputData(new_key);

  InfoHog<Name_t> infohog_new(DB);
  infohog_new.SetMember(&new_key, sizeof(new_key), 0);

  int stock_number;
  double price;

  cout << "Enter the items's stock number: ";
  cin >> stock_number;
  if(cin) {
    StockNumber_t sn = stock_number; 
    infohog_new.SetMember(&sn, sizeof(StockNumber_t), 1);
    cout << "Enter the items's price:       $";
    cin >> price;
  } 
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  if(cin) {
    Price_t p = price;
    infohog_new.SetMember(&p, sizeof(Price_t), 2);
  }
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  
  ClearInputStream(cin);
  int find = 0;

  if(change_object) { // Change the object
    if(!infohog.ChangeObject(infohog_new, find)) {
      cout << "Could not change this object" << "\n";
      return;
    }
    return;
  }
  
  // Change the object member by member
  if(!infohog.ChangeMember(infohog_new.GetMember(0), 
  			   infohog_new.GetMemberLen(0), 0, find)) {
    cout << "Could not change key member" << "\n";
    return;
  }
  if(!infohog.ChangeMember(infohog_new.GetMember(1), 
			   infohog_new.GetMemberLen(1), 1, find)) {
    cout << "Could not change member 1" << "\n";
    return;
  }
  if(!infohog.ChangeMember(infohog_new.GetMember(2), 
			   infohog_new.GetMemberLen(2), 2, find)) {
    cout << "Could not change member 2" << "\n";
    return;
  }
}

int main(int argv, char **argc)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

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
    // to the version set by the gxDatabaseRevisionLetter constant.  
  }

  InfoHogKey<Name_t> key_type;
  FStringCaseCompare = 0; // Do not compare the case of FString objects

  const char *data_file = "grocery.ihd";
  const char *index_file = "grocery.ihx";

  // Create or open an existing database using a single index file
  gxDatabaseAccessMode mode = gxDBASE_READWRITE;
  BtreeNodeOrder_t order = InfoHogNodeOrder;
  int num_trees = InfoHogNumTrees;
  int use_index = InfoHogUseIndexFile;
  FAU_t static_size = InfoHogStaticArea;
  __SBYTE__ df_rev_letter = rev_letter;
  __SBYTE__ if_rev_letter = rev_letter;
  
  POD pod;
  gxDatabaseError err = pod.Open(data_file, index_file, key_type,
				 order, mode, use_index, 
				 static_size, num_trees, df_rev_letter, 
				 if_rev_letter);

  // Test for any errors
  if(err != gxDBASE_NO_ERROR) {
    cout << gxDatabaseExceptionMessage(err) << "\n";
    return 1;
  }
  
  char c;
  int rv = 1;
  Menu();

  while(rv) {
    if (!cin) { 
      ClearInputStream(cin);
      if (!cin) { 
	cout << "Input stream error" << "\n";
	return 0;
      }
    }
    cout << '>';
    cin >> c;
    if (!cin) continue;
    switch(c) {
      case 'a' : case 'A' : 
	ClearInputStream(cin); AddItem(&pod); 
	break;
      case 'b' : case 'B' : 
	BuildDatabase(&pod); 
	break;
      case 'c' :
	ClearInputStream(cin); ChangeItem(&pod); 
	break;
      case 'C' :
	ClearInputStream(cin); ChangeItem(&pod, 0); 
	break;
      case 'd' : case 'D' : 
	ClearInputStream(cin); DeleteItem(&pod); 
	break;
      case 'f' : case 'F' : 
	ClearInputStream(cin); FindItem(&pod); 
	break;
      case 'l' : case 'L' : 
	ListInOrder(&pod); 
	break;	 
      case 'i' : case 'I' : 
	ListInOrder(&pod, 0); 
	break;
      case 'h' : case 'H' : case '?' : Menu(); break;
      case 'q' : case 'Q' : rv = Quit(); break;
      case 's' : case 'S' : PrintBtreeHeader(&pod, 0); break;
      case 'x' : case 'X' : 
	Compare(&pod); break;
      case 'y' : case 'Y' : 
	Rebuild(&pod, "newindex.ihx"); break;
      default:
        cout << "Unrecognized command" << "\n";
    }
  }

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
