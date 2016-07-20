// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Source Code File Name: grocery.cpp
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
*/
// ----------------------------------------------------------- //   
#include "gxdlcode.h"
#include "strutil.h"
#include "grocery.h"

Grocery::~Grocery()
{

}

void Grocery::SetName(const char *n)
{
  memset(name, MAX_NAME_LENGTH, 0);
  if(!n) return;
  strncpy(name, n,  MAX_NAME_LENGTH);
  name[MAX_NAME_LENGTH-1] = 0; // Ensure null termination
}

int Grocery::ChangeObject(const Grocery &ob)
{
  if(CaseICmp(name, ob.name) != 0) {
    if(!WriteName(ob.name)) return 0;
  }
    
  if(stock_number != ob.stock_number) {
    if(!WriteStockNumber(ob.stock_number)) return 0;
  }

  if(price != ob.price) {
    if(!WritePrice(ob.price)) return 0;
  }

  return 1; // No errors were encountered
}

int Grocery::WriteName(const char *n, int find_object)
// Write a new key name for this object without changing any other
// data members stored in the data file.
{
  if(!n) return 0;

  if(find_object) { // Obtain the objects datafile address and load the object
    if(!Find()) return 0; // Could not find the data file entry
    if(!ReadObject()) return 0;
  }
 if(UsingIndex()) {
   GroceryKey old_key((const char *)name, objectaddress, GetClassID());
   GroceryKey key(n, objectaddress, GetClassID());
   GroceryKey compare_key;
   if(!DeleteKey(old_key, compare_key)) return 0;
   if(!AddKey(key, compare_key)) return 0;
 }
 
 // Update this member
 SetName(n);
 
 FAU member_address = objectaddress + sizeof(gxObjectHeader);
  
 pod->OpenDataFile()->SeekTo(member_address);
 if(pod->GetDataFileError() != gxDBASE_NO_ERROR) {
   return 0;
 }
 
  // Write the member and check for errors
 if(pod->OpenDataFile()->Write(name, sizeof(name)) != gxDBASE_NO_ERROR) {
   return 0;
 }  
 
  // Flush the POD database following each key name change
 if(flush) pod->Flush(); 

 // The object was updated
 return 1;
}

int Grocery::WriteStockNumber(StockNumber_t n, int find_object, int flush_writes)
{
  if(find_object) { // Obtain the objects datafile address and load the object
    if(!Find()) return 0; // Could not find the data file entry
    if(!ReadObject()) return 0;
  }
  
  // Update this member
  stock_number = n;
  
  FAU member_address = objectaddress + sizeof(gxObjectHeader);
  member_address += sizeof(name);
  
  pod->OpenDataFile()->SeekTo(member_address);
  if(pod->GetDataFileError() != gxDBASE_NO_ERROR) {
    return 0;
  }

  // Write the member and check for errors
  if(pod->OpenDataFile()->Write(&stock_number, sizeof(stock_number), gxCurrAddress, 
				flush_writes) != gxDBASE_NO_ERROR) {
    return 0;
  }  

  // The object was updated
  return 1;
}

int Grocery::WritePrice(Price_t p, int find_object, int flush_writes)
{
  if(find_object) { // Obtain the objects datafile address and load the object
    if(!Find()) return 0; // Could not find the data file entry
    if(!ReadObject()) return 0;
  }
  
  // Update this member
  price = p;
  
  FAU member_address = objectaddress + sizeof(gxObjectHeader);
  member_address += (sizeof(name) + sizeof(stock_number));
  
  pod->OpenDataFile()->SeekTo(member_address);
  if(pod->GetDataFileError() != gxDBASE_NO_ERROR) {
    return 0;
  }

  // Write the member and check for errors
  if(pod->OpenDataFile()->Write(&price, sizeof(price), gxCurrAddress, 
				flush_writes) != gxDBASE_NO_ERROR) {
    return 0;
  }  

  // The object was updated
  return 1;
}

GroceryKey::GroceryKey() : DatabaseKeyB((char *)&key)
{
  memset(key.object_name, MAX_NAME_LENGTH, 0);
  key.object_id = (gxObjectID)0;
  key.class_id = (gxClassID)0;
}

GroceryKey::GroceryKey(const char *name, gxObjectID oid, gxClassID cid) : 
  DatabaseKeyB((char *)&key)
{
  strncpy(key.object_name, name,  MAX_NAME_LENGTH);
  key.object_name[MAX_NAME_LENGTH-1] = 0; // Ensure null termination
  key.object_id = oid;
  key.class_id = cid;
}

int GroceryKey::operator==(const DatabaseKeyB& k) const
{
  const GroceryKey *kptr = (const GroceryKey *)(&k);
  int rv = strcmp(key.object_name, kptr->key.object_name);
  return rv == 0;
}

int GroceryKey::operator>(const DatabaseKeyB& k) const
{
  const GroceryKey *kptr = (const GroceryKey *)(&k);
  int rv = strcmp(key.object_name, kptr->key.object_name);
  return rv > 0;
}

int GroceryKey::CompareKey(const DatabaseKeyB& k) const
// NOTE: This comparison function is only used if the 
// __USE_SINGLE_COMPARE__ preprocessor directive is 
// defined when the program is compiled.
{
  const GroceryKey *kptr = (const GroceryKey *)(&k);
  int rv = strcmp(key.object_name, kptr->key.object_name);
  return rv;
}

void GroceryKey::SetObjectName(const char *s)
{
  strncpy(key.object_name, s,  MAX_NAME_LENGTH);
  key.object_name[MAX_NAME_LENGTH-1] = 0; // Ensure null termination
}

Grocery &Grocery::operator=(const Grocery &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void Grocery::Copy(const Grocery &ob, int copy_pointers)
{
  SetName(ob.name);
  stock_number = ob.stock_number;
  price = ob.price;

  if(copy_pointers) {
    pod = ob.pod;
    objectaddress = ob.objectaddress;
  }
}

void Grocery::ClearName()
// Clears the name string.
{
  memset(name, MAX_NAME_LENGTH, 0);
}

int operator==(const Grocery &a, const Grocery &b)
{
  if(CaseICmp(a.name, b.name) != 0) return 0;
  if(a.stock_number != b.stock_number) return 0;
  if(a.price != b.price) return 0;

  return 1;
}

__UWORD__ Grocery::ObjectLength()
{
  __UWORD__ len = sizeof(name) + sizeof(stock_number) + sizeof(price);
  return len;
}

gxDatabaseError Grocery::Write()
{
  gxObjectHeader oh;

  // Allocate a block in the data file for this object's data
  objectaddress = pod->OpenDataFile()->Alloc(ObjectLength() +
					     sizeof(gxObjectHeader));
  
  // Check for any allocation errors
  if(objectaddress == (FAU_t)0) {
    return pod->GetDataFileError();
  }
  
  oh.ClassID = GetClassID();
  oh.ObjectID = objectaddress;

  // Write the object header to the datafile
  if(WriteObjectHeader(oh)!= gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }
  
  // Write the item name to the data file
  if(pod->OpenDataFile()->Write(name, sizeof(name)) != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }
  
  // Write the item's stock number to the data file
  if(pod->OpenDataFile()->Write(&stock_number, sizeof(stock_number)) !=
     gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }

  // Write the item's price to the data file
  if(pod->OpenDataFile()->Write(&price, sizeof(price)) != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }

  // Write a 32-bit CRC checksum for the object
  // NOTE: This step should only be preformed if the application
  // requires the use of persistent checksum values.
  pod->OpenDataFile()->WriteObjectChecksum(objectaddress);
  if(pod->GetDataFileError() != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }

  // Add the entry to the Index file
  if(UsingIndex()) {
    GroceryKey key(name, oh.ObjectID, oh.ClassID);
    GroceryKey compare_key;
    if(!AddKey(key, compare_key)) {
      return pod->GetIndexFileError();
    }
  }

  return gxDBASE_NO_ERROR;
}

gxDatabaseError Grocery::Read(FAU object_address)
{
  gxObjectHeader oh;

  // Optimize seeks during intervening reads
  pod->OpenDataFile()->SeekTo(object_address);
  if(pod->GetDataFileError() != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }
  
  if(ReadObjectHeader(oh, object_address) != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }
  
  // Incorrect object type
  // NOTE: This step should only be preformed if class ID values
  // are required by the application.
  if(oh.ClassID != GetClassID()) {
    return pod->SetDataFileError(gxDBASE_BAD_CLASS_ID);
  }
  
  // Read the object's name and check for errors
  if(pod->OpenDataFile()->Read(&name, sizeof(name)) !=
     gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }  
  
  // Read the object's stock number
  if(pod->OpenDataFile()->Read(&stock_number, sizeof(stock_number)) !=
     gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }  

  // Read the object's price
  if(pod->OpenDataFile()->Read(&price, sizeof(price)) != gxDBASE_NO_ERROR) {
    return pod->GetDataFileError();
  }

  objectaddress = object_address;

  return gxDBASE_NO_ERROR;
}

int Grocery::Find()
{
  // Search the index file for this entry
  if(UsingIndex()) {
    GroceryKey key((const char *)this->name);
    GroceryKey compare_key;
    if(!FindKey(key, compare_key)) return 0;
    objectaddress = key.ObjectID();
    return 1; // Found the index file entry
  }

  // If not using index file search the data file
  Grocery grocery;
  FAU_t oa;          // Object Address
  gxBlockHeader gx;  // Block Header
  gxObjectHeader oh; // Object Header
  
  FAU_t gxdfileEOF = pod->OpenDataFile()->GetEOF();
  FAU_t addr = (FAU_t)0;
  addr = pod->OpenDataFile()->FindFirstBlock(addr); // Search the entire file

  if(addr == (FAU_t)0) return 0; // No database blocks found in file

  memmove(grocery.name, this->name, MAX_NAME_LENGTH);
  grocery.stock_number = this->stock_number;
  grocery.price = this->price;

  while(1) { 
    if(FAU_t(addr + pod->OpenDataFile()->BlockHeaderSize()) >= gxdfileEOF) 
      break;
    if(pod->OpenDataFile()->Read(&gx, sizeof(gxBlockHeader), addr) !=
       gxDBASE_NO_ERROR) {
      return 0;
    }
    if(gx.block_check_word == gxCheckWord) {
      if((__SBYTE__)gx.block_status == gxNormalBlock) {
	oa = addr + pod->OpenDataFile()->BlockHeaderSize();
	if(ReadObjectHeader(oh, oa) != gxDBASE_NO_ERROR) {
	  return 0;
	}
	if(oh.ClassID == GetClassID()) { 
	  if(Read(oa) != gxDBASE_NO_ERROR) {
	    return 0;
	  }
	  if(strcmp(name, grocery.name) == 0) {
	    objectaddress = oa;
	    return 1; // Found unique data member
	  }
	}
      }
      addr = addr + gx.block_length; // Goto the next database block
    }
    else {
      addr++; // Keep searching until a valid database block is found
    }
  }

  // Reset the objects data
  memmove(this->name, grocery.name, MAX_NAME_LENGTH);
  this->stock_number = grocery.stock_number;
  this->price = grocery.price;
    
  return 0; // Could not find 
}

int Grocery::Delete()
{
  if(UsingIndex()) {
    GroceryKey key((const char *)this->name);
    GroceryKey compare_key;
    if(!FindKey(key, compare_key)) return 0; // Could not find the key
    objectaddress = key.ObjectID();
    if(!DeleteObject(objectaddress)) return 0; // Could not delete the object
    if(!DeleteKey(key, compare_key)) return 0; // Could not delete the key
    return 1; // The index and data file entry was deleted
  }

  // If not using index file search the data file
  if(!Find()) return 0; // Could not find the data file entry
  if(DeleteObject(objectaddress)) return 0; // Could not delete the object

  // The object was deleted from the data file
  return 1;
}

int Grocery::CompareIndex(unsigned index_number)
// Compares the data file to the index file.
// Returns true if data and index file match.
{
  if(!UsingIndex()) return 0;

  Grocery grocery(pod);
  GroceryKey key, compare_key;
  
  FAU_t oa;          // Object Address
  gxBlockHeader gx;  // Block Header
  gxObjectHeader oh; // Object Header
  
  int objects = 0; // Keeps track of good database blocks
  int matches = 0; // Keep track of matches
  
  FAU_t gxdfileEOF = pod->OpenDataFile()->GetEOF();
  FAU_t addr = (FAU_t)0;
  addr = pod->OpenDataFile()->FindFirstBlock(addr); // Search the entire file

  if(addr == (FAU_t)0) return 0; // No database blocks found in file

  while(1) { 
    if(FAU_t(addr + pod->OpenDataFile()->BlockHeaderSize()) >= gxdfileEOF) 
      break;
    pod->OpenDataFile()->Read(&gx, sizeof(gxBlockHeader), addr);
    if(gx.block_check_word == gxCheckWord) {
      if((__SBYTE__)gx.block_status == gxNormalBlock) {
	oa = addr + pod->OpenDataFile()->BlockHeaderSize();
	ReadObjectHeader(oh, oa);

	if(oh.ClassID == GetClassID()) { 
	  objects++; // Increment the object count
	  grocery.Read(oa);
	  key.SetObjectName(grocery.name);
	  key.SetObjectID(oa);
	  key.SetClassID(oh.ClassID);
	  if(FindKey(key, compare_key, index_number)) {
	    matches++; // Index and data file match
	  }
	  grocery.ClearName();
	}
      }
      addr = addr + gx.block_length; // Go to the next database block
    }
    else {
      addr++; // Keep searching until a valid database block is found
    }
  }

  return objects == matches;
}

int Grocery::RebuildIndexFile(const char *fname, unsigned index_number,
      			      int num_trees, BtreeNodeOrder_t node_order)
{
  if(!UsingIndex()) return 0;

  GroceryKey key, compare_key;
  gxBtree btx(key, node_order); 
  if(btx.Create(fname, num_trees) != gxDBASE_NO_ERROR) return 0;
  
  Grocery grocery(pod);
  
  FAU_t oa;          // Object Address
  gxBlockHeader gx;  // Block Header
  gxObjectHeader oh; // Object Header
  
  int objects = 0; // Keeps track of good database blocks
  int inserts = 0; // Keep track of inserts
  
  FAU_t gxdfileEOF = pod->OpenDataFile()->GetEOF();
  FAU_t addr = (FAU_t)0;
  addr = pod->OpenDataFile()->FindFirstBlock(addr); // Search the entire file

  if(addr == (FAU_t)0) return 0; // No database blocks found in file

  while(1) { 
    if(FAU_t(addr + pod->OpenDataFile()->BlockHeaderSize()) >= gxdfileEOF) 
      break;
    pod->OpenDataFile()->Read(&gx, sizeof(gxBlockHeader), addr);
    if(gx.block_check_word == gxCheckWord) {
      if((__SBYTE__)gx.block_status == gxNormalBlock) {
	oa = addr + pod->OpenDataFile()->BlockHeaderSize();
	ReadObjectHeader(oh, oa);
	if(oh.ClassID == GetClassID()) { 
	  objects++; // Increment the object count
	  grocery.Read(oa);
	  key.SetObjectName(grocery.name);
	  key.SetObjectID(oa);
	  key.SetClassID(oh.ClassID);
	  grocery.ClearName();
	   
	  // Could not add the key
	  if(!btx.Insert(key, compare_key)) return 0;
	  inserts++; // Index and data file match
	}
      }
      addr = addr + gx.block_length; // Go to the next database block
    }
    else {
      addr++; // Keep searching until a valid database block is found
    }
  }
  
  return objects == inserts;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
