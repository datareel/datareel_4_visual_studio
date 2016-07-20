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

This is a test program use to test the Persistent base class in a
multi-threaded application.
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "m_dbase.h"

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

void AddItem(MyDatabase *DB)
// Function used to add an object to the database.
{
  if(!InputData()) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery;
  grocery.SetName(Keybuffer::buf);

  if(DB->Find(grocery, 0)) {
    cout << "Item: " << Keybuffer::buf << " already exists" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
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
  
  if(!DB->Add(grocery, 0)) {
    cout << "Error adding object to database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
  }
}
 
void ChangeItem(MyDatabase *DB)
// Function used to change an object in the database.
{
  if(!InputData("Enter Item name to change")) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery;
  grocery.SetName(Keybuffer::buf);

  if(!DB->Find(grocery, 1)) {
    cout << "Could not find item: " << grocery.Name()
	 << " in the database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
    return;
  }

  cout << "Found item: " << Keybuffer::buf << "\n";
  
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

  if(!DB->Change(grocery, grocery_buf, 0)) {
    cout << "Error changing object in the database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
  }
  else {
    cout << "Object was changed, displaying updates below" << "\n";
    DB->Find(grocery, 1); // This call is for demo only
    DisplayItem(grocery);
  }
}

void FindItem(MyDatabase *DB)
// Function used to find an object in the database.
{
 if(!InputData()) {
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery;
  grocery.SetName(Keybuffer::buf);

  if(!DB->Find(grocery, 1)) {
    cout << "Could not find item: " << grocery.Name()
	 << " in the database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
    return;
  }

  cout << "Found item: " << Keybuffer::buf << "\n";

  DisplayItem(grocery);
}

void DeleteItem(MyDatabase *DB)
// Function used to delete an object from the database.
{
  if(!InputData()) { 
    cout << "No name was entered" << "\n";
    return;
  }

  Grocery grocery;
  grocery.SetName(Keybuffer::buf);

  if(!DB->Find(grocery, 0)) {
    cout << "Could not find item: " << grocery.Name()
	 << " in the database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
    return;
  }

  if(!DB->Delete(grocery, 0)) {
    cout << "Error deleting: " << Keybuffer::buf << " from the database" << "\n";
    cout << DB->LastErrorMessage() << "\n" << flush;
    return;
  }

  cout << "Deleted item: " << Keybuffer::buf << "\n";
}

void Menu()
// Console based user menu.
{
  cout << "(A, a)    Add object to the database" << "\n";
  //  cout << "(B, b)    Build a test database" << "\n";
  cout << "(C, c)    Change an existing database object" << "\n";
  cout << "(D, d)    Delete object from the database" << "\n";
  cout << "(F, f)    Find item by name" << "\n";          
  cout << "(L, l)    List without using index file" << "\n"; 
  cout << "(H, h, ?) Help (prints this menu)" << "\n";
  cout << "(I, i)    List using the index file" << "\n";
  cout << "(Q, q)    Quit" << "\n";
  //  cout << "(S, s)    Display B-tree stats" << "\n";
  //  cout << "(X, x)    Compare the index file to the data file" << "\n";
  //  cout << "(Y, y)    Rebuild the index file" << "\n";
}


void nt_ListInOrder(POD *DB);
void nt_ListInOrder(POD *DB, unsigned index_number);

int main(int argv, char **argc)
{

  // NOTE: The code below is for testing purposes only
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
  gxDatabaseAccessMode mode = gxDBASE_READWRITE;
  // NOTE: The code above is for testing purposes only

  // Create or open an existing database
  MyDatabase DB;
  if(!DB.Open(mode, rev_letter)) {
    cout << "Error opening the database" << "\n" << flush;
    cout << DB.LastErrorMessage() << "\n" << flush;
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
      case 'a' : case 'A' : ClearInputStream(cin); AddItem(&DB); break;
	//      case 'b' : case 'B' : BuildDatabase(&DB); break;
      case 'c' : case 'C' : ClearInputStream(cin); ChangeItem(&DB); break;
      case 'd' : case 'D' : ClearInputStream(cin); DeleteItem(&DB); break;
      case 'f' : case 'F' : ClearInputStream(cin); FindItem(&DB); break;
      case 'l' : case 'L' : nt_ListInOrder(DB.GetDB()); break;	 
      case 'i' : case 'I' : nt_ListInOrder(DB.GetDB(), 0); break;
      case 'h' : case 'H' : case '?' : 
	Menu(); 
	num_menu_displayed++;
	break;
      case 'q' : case 'Q' : rv = Quit(); break;
	// case 's' : case 'S' : PrintBtreeHeader(&BD, 0); break;
	// case 'x' : case 'X' : Compare(&DB); break;
	// case 'y' : case 'Y' : Rebuild(&DB, "newindex.btx"); break;

      default:
        cout << "Unrecognized command" << "\n";
    }
  }

  return 0;
}

void nt_ListInOrder(POD *DB, unsigned index_number)
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

void nt_ListInOrder(POD *DB)
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
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
