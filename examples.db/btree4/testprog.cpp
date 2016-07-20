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

This test program is used to store multiple gxBtrees in a single
index file.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxdstats.h"
#include "gxbtree.h"
#include "ustring.h"

const BtreeNodeOrder_t MyKeyClassOrder = 7;
const __WORD__ MyKeyNameSize = 64;

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
  key_name[MyKeyNameSize-1] = 0; // Ensure null termination
}

void MyKeyClass::operator=(const char *name)
{
  strncpy(key_name, name,  MyKeyNameSize);
  key_name[MyKeyNameSize-1] = 0; // Ensure null termination
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

const int NKEYS = 26;
const char *keys1[NKEYS] = { "DOG", "CAT", "FISH", "MOUSE", "BIRD", "PIG",
			     "HORSE", "LION", "SNAKE", "COW", "ARMADILLO",
			     "GROUPER", "RAT", "MONKEY", "ZEBRA",
			     "STARFISH", "LIZARD", "CRAB", "SNAIL",
			     "GORILLA", "LOBSTER", "TURKEY", "BEETLE",
			     "SHARK", "CLAM", "OYSTER" }; 

const char *keys2[NKEYS] = { "FLEA", "BUTTERFLY", "SPARROW", "GOLDFISH",
			     "TIGER", "BEAR", "TROUTE", "MOOSE", "DEAR",
			     "SALMON", "TUNA", "GAZELLE", "SLOTH", "SPIDER",
			     "LEAPORD", "GIRAFFE", "MUSTANG", "CONDOR",
			     "KANGAROO", "SKUNK", "FOX", "PANTER",
			     "CHEETAH", "TOUCAN", "PARROT", "BUFFALO" };


const char *keys3[NKEYS] = { "KOALA", "HORSEFLY", "ANACONDA", "CROCODILE",
			     "RACCOON", "ALLIGATOR", "RABBIT", "WHALE",
			     "ANT", "CRANE", "LONGHORN", "CANARY", "WOMBAT",
			     "WOLFHOUND", "COUGAR", "BAT", "OWL", "SHRIMP",
			     "SCALLOP", "SQUID", "PYTHON", "SARDINE",
			     "TAPIR", "ELEPHANT", "EEL", "RHINOCEROS" };

const char *keys4[NKEYS] = { "LAMB", "BISON", "GRASSHOPPER", "MACKEREL",
			     "FERRET", "WASP", "CATERPILLAR", "MILLIPEDE",
			     "CENTIPEDE", "MOSQUITO", "POSSUM", "DUCK",
			     "WEASEL", "CARIBOU", "ANTELOPE", "SALAMANDER",
			     "NEWT", "CHICKEN", "BULL", "COBRA",
			     "CHIMPANZEE", "URCHIN", "CROW", "WOLF",
			     "SPONGE", "JELLYFISH" };


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
  cout << "Size of header =    " << sizeof(gxBtreeHeader) << "\n";
  intbuf << clear << (FAU_t)btx.HeaderAddress();
  cout << "Header address =    " << intbuf.c_str() << "\n";
  intbuf << clear << (FAU_t)btx.Root();
  cout << "Root address =      " << intbuf.c_str() << "\n";
  cout << "Number of trees =   " << btx.NumTrees() << "\n";
  cout << "Number of entries = " << btx.NumKeys() << "\n";
  cout << "Number of nodes =   " << btx.NumNodes() << "\n";
  cout << "B-tree order =      " << btx.NodeOrder() << "\n";
  cout << "B-tree height =     " << btx.BtreeHeight() << "\n";
  PausePrg();
}

void BuildTree(gxBtree &btx, const char *keys[NKEYS])
{
  MyKeyClass key;
  MyKeyClass compare_key;
  int i, rv;

  const int INSERTIONS = NKEYS;

  // Inserting the keys
  for(i = 0; i < INSERTIONS; i++) {
    key = keys[i];  

    rv = btx.Insert(key, compare_key);
    
    if(rv != 1) {
      cout << "\n" << "Problem adding " << keys[i] << " - " << i << "\n";
      return;
    }
  }

  // "Verifying the insertions
  for(i = 0; i < INSERTIONS; i++) {
    key = keys[i];      
    rv = btx.Find(key, compare_key);
    if(rv != 1) {
      cout << "Error finding key " << keys[i] << " - " << i << "\n";
      return;
    }
  }

  // "Deleting all the entries
  for(i = 0; i < INSERTIONS; i++) {
    key = keys[i];      

    rv = btx.Delete(key, compare_key);

    if(rv != 1) {
      cout << "Error deleting key " << keys[i] << " - " << i << "\n";
      return;
    }

    // Verify the remaining key locations
    for(int j = INSERTIONS-1; j != i; j--) {
      key = keys[j];      
      rv = btx.Find(key, compare_key);
      if(rv != 1) {
	cout << "Error finding key " << keys[j] << " - " << j << "\n";
	cout << "After deleting key " << keys[i] << " - " << i << "\n";
	return;
      }
    }
  }

  // Re-inserting all the keys
  for(i = 0; i < INSERTIONS; i++) {
    key = keys[i];  

    rv = btx.Insert(key, compare_key);

    if(rv != 1) {
      cout << "\n" << "Problem adding " << keys[i] << " - " << i << "\n";
      return;
    }
  }

  BtreeStatus(btx);
}

int main(int argv, char **argc)
{
  int num_trees = 4; // Number of B-tree indexes this file will contain
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
    // to the version set by the gxDatabaseRevisionLetter constant.  
  }

  MyKeyClass key, compare_key;
  gxBtree btx(key, MyKeyClassOrder);

  // Create the B-tree with more than one tree
  btx.Create(fname, num_trees, rev_letter);
  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;

  cout << "Building B-tree number 1..." << "\n";
  // The create function will always connect to B-tree number 1
  BuildTree(btx, keys1);

  cout << "Building B-tree number 2..." << "\n";
  btx.ReConnect(2); // Connect to B-tree number 2
  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
  BuildTree(btx, keys2);
    
  cout << "Building B-tree number 3..." << "\n";
  btx.ReConnect(3); // Connect to B-tree number 3
  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
  BuildTree(btx, keys3);

  cout << "Building B-tree number 4..." << "\n";
  btx.ReConnect(4); // Connect to B-tree number 4
  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
  BuildTree(btx, keys4);

  int i;
  for(i = 1; i < (num_trees+1); i++) {
    cout << "Walking through the B-tree number " << i 
	 << " in sort order" << "\n";
    PausePrg();
    btx.ReConnect(i);
    if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
    if(btx.FindFirst(key)) {
      cout << key.key_name << ' ';
      while(btx.FindNext(key, compare_key))
	cout << key.key_name << ' ';
    }
    
    cout << "\n" << "\n";
  }

  cout << "Closing and reopening the B-tree index file" << "\n";
  btx.Open(fname);

  if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
  num_trees = btx.NumTrees();

  for(i = 1; i < (num_trees+1); i++) {
    cout << "Walking through the B-tree number " << i 
	 << " in sort order" << "\n";
    PausePrg();
    btx.ReConnect(i);
    if(CheckError(btx.gxDatabasePtr()) != 0) return 1;
    if(btx.FindFirst(key)) {
      cout << key.key_name << ' ';
      while(btx.FindNext(key, compare_key))
	cout << key.key_name << ' ';
    }
    
    cout << "\n" << "\n";
  }

  cout << "Exiting..." << "\n";
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
