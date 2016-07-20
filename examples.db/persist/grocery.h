// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: grocery.h
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997
// Date Last Modified: 01/29/2009
// Copyright (c) 2001-2009 DataReel Software Development
// ----------------------------------------------------------- // 
// ---------- Include File Description and Details  ---------- // 
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
*/
// ----------------------------------------------------------- //   
#ifndef __GROCERY_HPP__
#define __GROCERY_HPP__

#include "gxdlcode.h"

#include "gpersist.h"
#include "gxfloat.h"

// Constants
const unsigned MAX_NAME_LENGTH = 64;   // Max object name length
const gxClassID ClassGroceryID = 1024; // Arbitrary class ID

// Type definitions
typedef gxINT32 StockNumber_t; // Integer
typedef gxFLOAT64 Price_t;     // Floating point


// Primary key type used by this database
struct GroceryKey_t
{
  char object_name[MAX_NAME_LENGTH]; // Object name
  gxObjectID object_id; // Object data file address/identification number
  gxClassID class_id;   // Object class identification number
};

// Primary key class used by this database 
class GroceryKey : public DatabaseKeyB
{
public:
  GroceryKey();
  GroceryKey(const char *name, gxObjectID oid = (gxObjectID_t)0,
	     gxClassID cid = (gxClassID_t)0);
  
public: // Base class interface
  size_t KeySize() { return sizeof(key); }
  int operator==(const DatabaseKeyB& k) const;
  int operator>(const DatabaseKeyB& k) const;

  // NOTE: This comparison function is only used if the 
  // __USE_SINGLE_COMPARE__ preprocessor directive is 
  // defined when the program is compiled.
  int CompareKey(const DatabaseKeyB& key) const;

public:
  char *ObjectName() { return key.object_name; }
  gxObjectID ObjectID() const { return key.object_id; }
  gxClassID ClassID() const { return key.class_id; }
  void SetObjectName(const char *s);
  void SetObjectID(gxObjectID oid) { key.object_id = oid; }
  void SetClassID(gxClassID cid) { key.class_id = cid; }
  
private:
  GroceryKey_t key;
};

// Persistent object database
class Grocery : public gxPersistent
{ 
public:
  Grocery(POD *podfile) : gxPersistent(podfile) {
    ClearName(); stock_number = (StockNumber_t)0; price = (Price_t)0;
  }
  Grocery(const POD *podfile) : gxPersistent(podfile) {
    ClearName(); stock_number = (StockNumber_t)0; price = (Price_t)0;
  }
  Grocery() {
    ClearName(); stock_number = (StockNumber_t)0; price = (Price_t)0;
  }
  Grocery(const Grocery &ob) { Copy(ob); }
  Grocery &operator=(const Grocery &ob);

  virtual ~Grocery();  

public: // Memeber access
  StockNumber_t StockNumber() { return stock_number; }
  Price_t Price() { return price; }
  char *Name() { return name; }
  void SetStockNumber(StockNumber_t n) { stock_number = n; }
  void SetPrice(Price_t p) { price = p; }
  void SetName(const char *n);
  void ClearName();

public: // Helper functions
  void Copy(const Grocery &ob, int copy_pointers = 1);

public: // Advanced database functions
  int ChangeObject(const Grocery &ob);
  int WriteName(const char *n, int find_object = 1);
  int WriteStockNumber(StockNumber_t n, int find_object = 1, int flush_writes = 1);
  int WritePrice(Price_t p, int find_object = 1, int flush_writes = 1);

public: // Database utility functions
  int CompareIndex(unsigned index_number);
  int RebuildIndexFile(const char *fname, unsigned index_number,
		       int num_trees, BtreeNodeOrder_t node_order);

public: // Runtime type information functions
  gxClassID GetClassID() { return ClassGroceryID; }
  const char *GetClassName() { return (const char *)"Class Grocery"; }
  
public: // Operator overloads
  friend int operator==(const Grocery &a, const Grocery &b);

private: // Base class interface
  virtual gxDatabaseError Write();
  virtual gxDatabaseError Read(FAU Address);
  virtual int Find();
  virtual int Delete();
  virtual __UWORD__ ObjectLength();

private:
  // NOTE: Do not change the ordering of the data members in order to
  // maintain a uniform byte alignment between the in-memory copy of
  // the object and the copy stored on disk. If the member alignment
  // is changed all database files using this sturcture will have to
  // be rebuilt.
  char name[MAX_NAME_LENGTH];
  StockNumber_t stock_number; 
  Price_t price;
};

#endif // __GROCERY_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
