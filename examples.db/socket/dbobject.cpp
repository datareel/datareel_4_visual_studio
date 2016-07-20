// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: dbobject.cpp
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

Database object class used with the client/server example.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#include "dbobject.h"

DatabaseObjectData::DatabaseObjectData()
{
  oid = (OID)0;
  cid = (CID)0;
  for(int i = 0; i < gxMaxNameLength; i++) name[i] = 0;
}

FAU_t DatabaseObject::WriteObject()
{
  FAU_t addr = Alloc(sizeof(DatabaseObjectData));
  if(addr == (FAU_t)0) return (FAU_t)0;
  
  Write(p, sizeof(DatabaseObjectData));
  if(gxd_error != gxDBASE_NO_ERROR) return (FAU_t)0;

  return addr;
}

int DatabaseObject::ReadObject(FAU_t Address)
{
  // Optimize seeks during intervening reads
  SeekTo(Address);
  Read((char *)this->p, sizeof(DatabaseObjectData));
  if(gxd_error != gxDBASE_NO_ERROR) return 0;
  return 1;
}

FAU_t DatabaseObject::FindObject()
{ 
  // If not using index file or entry not found, search the data file
  DatabaseObjectData dbobject;
  dbobject.oid = p->oid;
  dbobject.cid = p->cid;
  strcpy(dbobject.name, p->name);

  FAU_t object_address = (FAU_t)0;

  while(1) {
    object_address = FindFirstObject(object_address);
    if(object_address == (FAU_t)0) break;
    ReadObject(object_address);
    if(gxd_error == gxDBASE_NO_ERROR) {
      if(strcmp(p->name, dbobject.name) == 0) {
	return object_address; // Found unique data member
      }
    }
    object_address++; // Offset the block address
  }

  // Reset the objects data
  p->oid = dbobject.oid;
  p->cid = dbobject.cid;
  strcpy(p->name, dbobject.name);
  
  return (FAU_t)0; // Could not find 
}

FAU_t DatabaseObject::ChangeObject(const DatabaseObjectData &new_dbobject_info)
{
  FAU_t addr = FindObject();
  if(addr == (FAU_t)0) return (FAU_t)0; // Object does not exist
  Write(&new_dbobject_info, sizeof(new_dbobject_info), addr);
  if(gxd_error != gxDBASE_NO_ERROR) return (FAU_t)0;
  strcpy(p->name, new_dbobject_info.name);
  p->oid = new_dbobject_info.oid;
  p->cid = new_dbobject_info.cid;
  return addr;
}

FAU_t DatabaseObject::DeleteObject()
{
  FAU_t addr = FindObject();
  if(addr == (FAU_t)0) return (FAU_t)0; // Object does not exist
  Delete(addr);
  if(gxd_error != gxDBASE_NO_ERROR) return (FAU_t)0;
  return addr;
}

FAU_t DatabaseObject::RemoveObject()
{
  FAU_t addr = FindObject();
  if(addr == (FAU_t)0) return (FAU_t)0; // Object does not exist
  Remove(addr);
  if(gxd_error != gxDBASE_NO_ERROR) return (FAU_t)0;
  return addr;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
