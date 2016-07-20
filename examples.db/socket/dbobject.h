// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: dbobject.h
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/18/1997  
// Date Last Modified: 01/01/2009
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

Database object class used with the client/server example.
*/
// ----------------------------------------------------------- //   
#ifndef __GX_S_DATABASEOBJECT_HPP__
#define __GX_S_DATABASEOBJECT_HPP__

#include "gxdlcode.h"

#include <string.h>
#include "gxdbase.h"
#include "gxfloat.h"

typedef gxINT32 OID;
typedef gxFLOAT64 CID;

struct DatabaseObjectData {
  DatabaseObjectData();

  char name[gxMaxNameLength];  
  OID oid; 
  CID cid;
};

class DatabaseObject : public gxDatabase
{ 
public:
  DatabaseObject() { p = new DatabaseObjectData; }
  ~DatabaseObject() { delete p; }

public: 
  FAU_t WriteObject();
  int ReadObject(FAU_t Address);
  FAU_t FindObject();
  FAU_t DeleteObject();
  FAU_t RemoveObject();
  FAU_t ChangeObject(const DatabaseObjectData &new_dbobject_info);

public:
  DatabaseObjectData *p;
};

#endif // __GX_S_DATABASEOBJECT_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
