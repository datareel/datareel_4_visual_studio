// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997
// Date Last Modified: 01/29/2009
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

This is a test program use to test the Persistent base class.

This example uses a fixed string as the key type and incorporates
an object ID and class ID into the data key. NOTE: The use a
fixed character string as a key type is intended to keep this
example as simple as possible and used for benchmarking
performance in the worst case scenario. Character strings are
hard to manipulate (in memory and on disk) and using the strcmp()
function is extremely slow.  

The object ID in this example is used to uniquely identify each
object in the B-tree index and record the object's location in
the data file. Using a data file address to ID objects guarantees
that each object will be unique.

The class ID in this example is an optional parameter used in
place of RTTI. This optional feature allows you determine the
object type without entering the object. When the Persistent base
class was originally developed in 1997 RTTI was not supported by
some C++ compilers so this feature was implemented to allow
applications to obtain type information and used by the Persistent 
base class to store objects from multiple classes in a single
database.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "grocery.h"

struct Keybuffer {
  static char buf[MAX_NAME_LENGTH];
};

// NOTE: Using static member with no locking protocol for example only. 
char Keybuffer::buf[MAX_NAME_LENGTH];

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

int InputData(const char *prompt)
// Function used to read a string from the input stream.
{
  memset(Keybuffer::buf, MAX_NAME_LENGTH, 0);
  if(prompt) cout << prompt << ": ";
  cin.getline(Keybuffer::buf, MAX_NAME_LENGTH);
  int count = cin.gcount();
  if(count > 0) count--; // Account for Enter key
  return count; // Return the number of chars entered
}

int InputData()
{
  const char *prompt = "Item name";
  return InputData(prompt);
}

void DisplayItem(Grocery &grocery, int full = 1)
// Function used to print a grocery list object to the stdout.
{
  cout << "\n";
  cout << "Item's Name  = " << grocery.Name() << "\n";
  if(full) {
    cout << "Stock Number = " << grocery.StockNumber() << "\n";
    cout.setf(ios::showpoint | ios::fixed);
    cout.precision(2);
    cout << "Price        = $" << grocery.Price() << "\n";
    cout << "\n";
  }
}

void AddItem(POD *DB)
// Function used to add an object to the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  if(!InputData()) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery(DB);
  grocery.SetName(Keybuffer::buf);
  int exists = grocery.FindObject();

  if(exists) {
    cout << "Item: " << Keybuffer::buf << " already exists" << "\n";
    return;
  }

  int stock_number;
  double price;

  cout << "Enter the items's stock number: ";
  cin >> stock_number;
  if(cin) {
    grocery.SetStockNumber(stock_number);
    cout << "Enter the items's price:       $";
    cin >> price;
  } 
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  if(cin) {
    grocery.SetPrice(price);
  }
  else {
    cout << "Invalid entry. Object not added!" << "\n";
    return;
  }
  
  ClearInputStream(cin);
  
  if(!grocery.WriteObject()) {
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

  if(!InputData()) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery(DB);
  grocery.SetName(Keybuffer::buf);
  if(!grocery.DeleteObject()) {
    cout << "Could not find item: " << Keybuffer::buf << " in the database" << "\n";
    return;
  }

  cout << "Deleted item: " << Keybuffer::buf << "\n";

  DB->Flush(); // Flushing the database buffers following manual deletion
}

void PrintBtreeHeader(POD *DB, unsigned index_number) 
{
  gxBtreeHeader hdr;
  DB->Index(index_number)->ReadBtreeHeader(hdr);
  cout << "B-tree header information" << "\n";

  cout << "Node order:      " << hdr.node_order << "\n";
  cout << "Key size:        " << hdr.key_size << "\n";
  cout << "Number of nodes: " << hdr.n_nodes << "\n";
  cout << "B-tree height:   " << hdr.btree_height << "\n";
  cout << "Root Address:    " << (long)hdr.root << "\n";
  // NOTE: The root node FAU is cast to a long for compilers
  // with native 64-bit int ostream overloads
  cout << "Number of trees: " << hdr.num_trees << "\n";
  cout << "\n";
}

void ListInOrder(POD *DB, unsigned index_number)
// List the contents of the database using the index file.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  Grocery grocery(DB);
  GroceryKey key, compare_key;
  gxBtree *btx = DB->Index(index_number);

  // Walk through the tree starting at the first key
  if(btx->FindFirst(key)) {
    if(!grocery.ReadObject(key.ObjectID())) {
      cout << "Error reading the object" << "\n";
      return;
    }
    DisplayItem(grocery);
    grocery.ClearName();
    
    while(btx->FindNext(key, compare_key)) {
      if(!grocery.ReadObject(key.ObjectID())) {
	cout << "Error reading the object" << "\n";
	return;
      }
      DisplayItem(grocery);
      grocery.ClearName();
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
  Grocery grocery(DB);
  unsigned count = 0;
  ClearInputStream(cin); // Clear input stream
  while(1) {
    addr = DB->OpenDataFile()->FindFirstObject(addr);
    if(!addr) break;
    if(!grocery.ReadObject(addr)) {
      cout << "Error reading the object" << "\n";
      return;
    }
    DisplayItem(grocery);
    grocery.ClearName();
    count++;
    if(count == 2) {
      PausePrg();
      count = 0;
    }
  }
}

void ChangeItem(POD *DB)
// Function used to find an object in the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  if(!InputData("Enter Item name to change")) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery(DB);
  grocery.SetName(Keybuffer::buf);
  if(!grocery.FindObject()) {
    cout << "Could not find item: " << grocery.Name()
	 << " in the database" << "\n";
    return;
  }

  cout << "Found item: " << Keybuffer::buf << "\n";
  
  if(!grocery.ReadObject()) {
    cout << "Error reading the object" << "\n";
    return;
  }

  // Make a copy of the current object in a temp buffer 
  Grocery grocery_buf = grocery;

  cout << "Change field at the prompt or press Enter to skip field" << "\n"; 
  cout << "Item's Name [" << grocery.Name() << "]: ";
  if(InputData(0)) grocery_buf.SetName(Keybuffer::buf);

  cout << "Stock Number [" << grocery.StockNumber() << "]: ";
  if(InputData(0)) grocery_buf.SetStockNumber((StockNumber_t)atoi(Keybuffer::buf));

  cout.setf(ios::showpoint | ios::fixed);
  cout.precision(2);
  cout << "Price $[" << grocery.Price() << "]: ";
  if(InputData(0)) grocery_buf.SetPrice((Price_t)atof(Keybuffer::buf));

  if(grocery == grocery_buf) {
    // NOTE: This check is only made to alert the user.
    // NOTE: The ChangeObject() function will not write 
    // NOTE: to the datbase if there are no changes to 
    // NOTE: commit.
    cout << "There are no changes to commit" << "\n";
    return;
  }

  cout << "Commiting changes" << "\n";

  if(!grocery.ChangeObject(grocery_buf)) {
    cout << "Error changing object in the database" << "\n";
  }
  else {
    cout << "Object was changed, displaying updates below" << "\n";
    grocery.FindObject();
    DisplayItem(grocery);
  }
}

void FindItem(POD *DB)
// Function used to find an object in the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

 if(!InputData()) {
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery(DB);
  grocery.SetName(Keybuffer::buf);
  if(!grocery.FindObject()) {
    cout << "Could not find item: " << grocery.Name()
	 << " in the database" << "\n";
    return;
  }

  cout << "Found item: " << Keybuffer::buf << "\n";
  
  if(!grocery.ReadObject()) {
    cout << "Error reading the object" << "\n";
    return;
  }
  DisplayItem(grocery);
}

void BuildDatabase(POD *DB)
// Function used to build a test database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  DB->TestDatabase();

  const int NUM_OBJECTS = 1000;
  int i;
  Grocery grocery(DB);
  char name[MAX_NAME_LENGTH];
  int stock_number = 2000;
  double price = 0.95;

  cout << "Adding " << NUM_OBJECTS << " objects to the database..." << "\n";
  for(i = 0; i < NUM_OBJECTS; i++) {
    sprintf(name, "Item %i", i);

    // Set the item's name, stock number, price
    grocery.SetName(name);
    grocery.SetStockNumber(StockNumber_t(stock_number+i));
    grocery.SetPrice(Price_t(price+i));

    // Write the grocery list item to the database
    if(!grocery.WriteObject()) {
      cout << "Could not add object number " << i << " to the database"
	   << "\n";
      return;
    }
  }
  
  // Flush the database files after a batch insert
  DB->Flush();
}
 
void Menu()
// Console based user menu.
{
  cout << "(A, a)    Add object to the database" << "\n";
  cout << "(B, b)    Build a test database" << "\n";
  cout << "(C, c)    Change an existing database object" << "\n";
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

  Grocery grocery(DB);
  cout << "\n";
  cout << "Comparing the index file to the data file..." << "\n";
  int rv = grocery.CompareIndex(0);
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

  Grocery grocery(DB);
  cout << "\n";
  cout << "Rebuilding the index file..." << "\n";
  // Rebuild index number 0 with room for 1 tree header
  int rv = grocery.RebuildIndexFile(fname, 0, 1, POD_DEFAULT_ORDER);
  if(!rv) {
    cout << "The index file was not rebuilt!" << "\n";
    cout << "\n";
    return;
  }

  cout << "The index file was rebuilt." << "\n";
  cout << "A new index file named " << fname << " was created.";
  cout << "\n";
}

int main(int argv, char **argc)
{
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

  const char *data_file = "grocery.gxd";
  const char *index_file = "grocery.btx";
  GroceryKey key_type;

  // Create or open an existing database using a single index file
  POD pod;

  gxDatabaseAccessMode mode = gxDBASE_READWRITE;
  BtreeNodeOrder_t order = POD_DEFAULT_ORDER;
  int num_trees = 1; // Use one tree per index file
  int use_index = 1;
  FAU_t static_size = (FAU_t)0;
  __SBYTE__ df_rev_letter = rev_letter;
  __SBYTE__ if_rev_letter = rev_letter;
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
  int num_menu_displayed = 1;
  Menu();
  
  while(rv) {
    if (!cin) { 
      ClearInputStream(cin);
      if (!cin) { 
	cout << "Input stream error" << "\n";
	return 0;
      }
    }
    if(num_menu_displayed >= 1) {
      cout << '>';
    }
    else {
      cout << "Press ? or h for help>";
    }
    if(num_menu_displayed > 0) num_menu_displayed--;

    cin >> c;
    if (!cin) continue;
    switch(c) {
      case 'a' : case 'A' : ClearInputStream(cin); AddItem(&pod); break;
      case 'b' : case 'B' : BuildDatabase(&pod); break;
      case 'c' : case 'C' : ClearInputStream(cin); ChangeItem(&pod); break;
      case 'd' : case 'D' : ClearInputStream(cin); DeleteItem(&pod); break;
      case 'f' : case 'F' : ClearInputStream(cin); FindItem(&pod); break;
      case 'l' : case 'L' : ListInOrder(&pod); break;	 
      case 'i' : case 'I' : ListInOrder(&pod, 0); break;
      case 'h' : case 'H' : case '?' : 
	Menu(); 
	num_menu_displayed++;
	break;
      case 'q' : case 'Q' : rv = Quit(); break;
      case 's' : case 'S' : PrintBtreeHeader(&pod, 0); break;
      case 'x' : case 'X' : Compare(&pod); break;
      case 'y' : case 'Y' : Rebuild(&pod, "newindex.btx"); break;

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
