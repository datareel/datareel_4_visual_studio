// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Header File Name: gxrdbcon.h
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 08/09/2001
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

gxRDBMS functions shared by console based test programs.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <time.h>
#include "gxrdbms.h"
#include "gxrdbdef.h"
#include "gxdstats.h"

// Console functions
void PausePrg();
void ClearInputStream(GXSTD::istream &s);

// gxRDBMS fucntions
void AddRecord(gxRDBMS *rdb);
void ChangeOrAdd(gxRDBMS *rdb);
void ChangeRecord(gxRDBMS *rdb);
void DeleteRecord(gxRDBMS *rdb);
int DisplayRecord(gxRDBMS *rdb, gxrdDatabaseRecord &record);
void DumpDatabase(gxRDBMS *rdb);
void DumpDatabase(gxRDBMS *rdb, unsigned index_number);
void FindRecord(gxRDBMS *rdb);
int InputRecordData(gxRDBMS *rdb, gxrdDatabaseRecord &record, 
		    int clear_record = 1);

// gxRDBMS statistical functions
void PrintKeyInfo(gxRDBMS *rdb);
void PrintBtreeHeader(gxBtreeHeader &hdr);
void PrintBtreeHeader(gxRDBMS *rdb);

// Database fault detection and recovery functions
void CheckDataFile(gxRDBMS *rdb);
int CheckError(gxRDBMS *rdb);
void CheckIndex(gxRDBMS *rdb);
int DatabaseAutoRecover(gxRDBMS *rdb, int recover_datafile, int indexnum);
int DatabaseConsistencyCheck(gxRDBMS *rdb, int auto_recover, int force_verify);
void RebuildDataFile(gxRDBMS *rdb);
void RebuildIndex(gxRDBMS *rdb);

// Test functions
void BatchDelete(gxRDBMS *rdb, unsigned index_number = 0);
void BatchInsert(gxRDBMS *rdb);
int LoadTestRecord(gxRDBMS *rdb, gxrdDatabaseRecord &record,
		   __ULWORD__ i);
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
