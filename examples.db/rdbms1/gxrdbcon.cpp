// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: gxrdbcon.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 08/09/2001
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

gxRDBMS functions shared by console based test programs.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#include "gxrdbcon.h"

void PausePrg()
// Function used to pause the program.
{
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Press enter to continue..." << "\n" << GXSTD::flush;
  GXSTD::cin.get();
}

void ClearInputStream(GXSTD::istream &s)
// Function used to clear the input stream.
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

int DisplayRecord(gxRDBMS *rdb, gxrdDatabaseRecord &record)
// Function used to display a gxRDBMS record. Returns true 
// if the record is displayed with no errors.
{
  int i, fs, type, column_length;
  gxString field_name;

  // Table type buffers
  gxString sbuf;
  gxINT16 i16buf;
  gxINT32 i32buf;
  gxFLOAT64 f64buf;
  CDate date_buf;
  char bool_buf;

#if defined (__64_BIT_DATABASE_ENGINE__)
  gxINT64 i64buf;
#else
  gxINT32 low_word, high_word;
  char *ptr;
#endif

  for(i = 0; i < rdb->Table().table_header.num_fields; i++) {
    // NOTE: The column length is not used by console applications.
    // This value is used to tell GUI's how many table characters to 
    // allocate per table field.
    column_length = rdb->Table().table_header.fields[i].column_length;
    fs = (int)rdb->Table().table_header.fields[i].format_specifier;
    field_name << clear << rdb->Table().table_header.fields[i].field_name;
    type = (int)rdb->Table().table_header.fields[i].field_type_id;

    switch(type) {
      case gxrdBOOL :
	GXSTD::cout << field_name.c_str() << ": ";
	record.GetField(&bool_buf, sizeof(bool_buf), i);
	GXSTD::cout << bool_buf << "\n" << GXSTD::flush;
	break;

      case gxrdCHAR : case gxrdVCHAR :
	GXSTD::cout << field_name.c_str() << ": ";
	if(record.FieldHasNull(i)) {
	  sbuf << clear << (char *)record.FieldData(i);
	}
	else {
	  sbuf.Clear();
	  sbuf.InsertAt(0, (char *)record.FieldData(i), 
			record.FieldLength(i));
	}
	GXSTD::cout << sbuf.c_str() << "\n" << GXSTD::flush;
	break;

      case gxrdBINARY : 
	GXSTD::cout << field_name.c_str() << "\n" << GXSTD::flush;
	GXSTD::cout << "This program has stored a string in the user \
defined space" << "\n" << "String: ";
	GXSTD::cout.write((char *)record.FieldData(i), record.FieldLength(i));
	GXSTD::cout << "\n" << GXSTD::flush;
	break;

      case gxrdBLOB :
	GXSTD::cout << field_name.c_str() << "\n" << GXSTD::flush;
	GXSTD::cout << "This program has stored a string in the BLOB space" 
		    << "\n" << "String: ";
	GXSTD::cout.write((char *)record.FieldData(i), record.FieldLength(i));
	GXSTD::cout << "\n" << GXSTD::flush;
	break;

      case gxrdINT16 :
	GXSTD::cout << field_name.c_str() << ": ";
	record.GetField(&i16buf, sizeof(i16buf), i);
	GXSTD::cout << i16buf << "\n" << GXSTD::flush;
	break;

      case gxrdINT32 :
	GXSTD::cout << field_name.c_str() << ": ";
	record.GetField(&i32buf, sizeof(i32buf), i);
	GXSTD::cout << i32buf << "\n" << GXSTD::flush;
	break;

      case gxrdINT64 :
#if defined (__64_BIT_DATABASE_ENGINE__)
	GXSTD::cout << field_name.c_str() << ": ";
	record.GetField(&i64buf, sizeof(i64buf), i);
	sbuf << clear << i64buf;
	GXSTD::cout << sbuf.c_str() << "\n" << GXSTD::flush;
#else
	GXSTD::cout << field_name.c_str() << ": ";
	ptr = (char *)record.FieldData(i);
	memmove(&low_word, ptr, sizeof(low_word));
	memmove(&high_word, (ptr+sizeof(low_word)), sizeof(high_word));
	GXSTD::cout << "HighWord = " << high_word << " LowWord = " << low_word
		    << "\n" << GXSTD::flush;
#endif
	break;

      case gxrdFLOAT : 
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cout.setf(GXSTD::ios::showpoint | GXSTD::ios::fixed);
	if(fs > 0) { 
	  // Display a specified number of digits after the decimal
	  // point.
	  GXSTD::cout.precision(fs);
	}
	record.GetField(&f64buf, sizeof(f64buf), i);
	GXSTD::cout << f64buf << "\n" << GXSTD::flush;
	break;

      case gxrdCURRENCY :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cout.setf(GXSTD::ios::showpoint | GXSTD::ios::fixed);
	if(fs == gxrdDollar) {
	  GXSTD::cout << "$";
	}
	GXSTD::cout.precision(2);
	record.GetField(&f64buf, sizeof(f64buf), i);
	GXSTD::cout << f64buf << "\n" << GXSTD::flush;
	break;

      case gxrdDATE :
	GXSTD::cout << field_name.c_str() << ": ";
	record.GetField(&date_buf, sizeof(date_buf), i);
	GXSTD::cout << date_buf << "\n" << GXSTD::flush;
	break;
      
      default:
	GXSTD::cout << "Bad table type" << "\n" << GXSTD::flush;
	return 0;
    }
  }
  GXSTD::cout << "\n" << GXSTD::flush;

  return 1;
}

int InputRecordData(gxRDBMS *rdb, gxrdDatabaseRecord &record, 
		    int clear_record)
// Function used to load data into a gxRDBMS record. Returns true if the 
// record is loaded with no errors.
{
  int i, type, column_length, fs;
  gxString field_name;
  __ULWORD__ length;

  // Table type buffers
  gxString sbuf, cbuf64;
  gxINT16 i16buf;
  gxINT32 i32buf;
  gxFLOAT64 f64buf;
  short ci16buf;
  long ci32buf;
  double cf64buf;
  CDate date_buf;
  char bool_buf;
  
#if defined (__64_BIT_DATABASE_ENGINE__)
  gxINT64 i64buf;
#else
  char *ptr;
  long clow_word, chigh_word;
  gxINT32 low_word, high_word;
#endif

  if(clear_record) {
    record.Clear();
    record.CreateRecord(rdb->Table().record_def);
  }

  for(i = 0; i < rdb->Table().table_header.num_fields; i++) {
    // NOTE: The column length is not used by console applications.
    // This value is used to tell GUI's how many table characters to 
    // allocate per table field.
    column_length = rdb->Table().table_header.fields[i].column_length;
    fs = (int)rdb->Table().table_header.fields[i].format_specifier;
    field_name << clear << rdb->Table().table_header.fields[i].field_name;
    type = (int)rdb->Table().table_header.fields[i].field_type_id;
    switch(type) {
      case gxrdBOOL :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> bool_buf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	record.SetField(&bool_buf, sizeof(bool_buf), i);
	break;

      case gxrdCHAR : case gxrdVCHAR :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> sbuf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	length = sbuf.length();
	if(type == gxrdCHAR) {
	  // Ensure fixed length strings stay inbounds
	  if(length > record.FieldLength(i)) length = record.FieldLength(i);
	}
	record.SetField(sbuf.GetSPtr(), length, i);
	break;

      case gxrdBINARY : 
	GXSTD::cout << field_name.c_str() << "\n" << GXSTD::flush;
	GXSTD::cout << "Enter a string to store in the user defined space" 
		    << "\n" << "String: ";
	GXSTD::cin >> sbuf;
	
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	record.SetField(sbuf.GetSPtr(), sbuf.length(), i);
	break;

      case gxrdBLOB :
	GXSTD::cout << field_name.c_str() << "\n" << GXSTD::flush;
	GXSTD::cout << "Enter a string to store in the BLOB space" 
		    << "\n" << "String: ";
	GXSTD::cin >> sbuf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	record.SetField(sbuf.GetSPtr(), sbuf.length(), i);
	break;

      case gxrdINT16 :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> ci16buf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	i16buf = ci16buf;
	record.SetField(&i16buf, sizeof(i16buf), i);
	break;

      case gxrdINT32 :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> ci32buf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	i32buf = ci32buf;
	record.SetField(&i32buf, sizeof(i32buf), i);
	break;
	
      case gxrdINT64 :
#if defined (__64_BIT_DATABASE_ENGINE__)
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> cbuf64;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	i64buf = cbuf64.Atoi64();
	record.SetField(&i64buf, sizeof(i64buf), i);
#else 
	GXSTD::cout << field_name.c_str() << "\n" << GXSTD::flush;
	GXSTD::cout << "Enter low word value: ";
	GXSTD::cin >> clow_word;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	GXSTD::cout << "Enter high word value: ";
	GXSTD::cin >> chigh_word;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	low_word = clow_word; high_word = chigh_word;
	ptr = (char *)record.FieldData(i);
	memmove(ptr, &low_word, sizeof(low_word));
	memmove((ptr+sizeof(low_word)), &high_word, sizeof(high_word));
#endif
	break;

      case gxrdFLOAT : case gxrdCURRENCY :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> cf64buf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	f64buf = cf64buf;
	record.SetField(&f64buf, sizeof(f64buf), i);
	break;
      
      case gxrdDATE :
	GXSTD::cout << field_name.c_str() << ": ";
	GXSTD::cin >> date_buf;
	if(!GXSTD::cin) {
	  GXSTD::cout << "Invalid entry!" << "\n" << GXSTD::flush;
	  return 0;
	}
	record.SetField(&date_buf, sizeof(date_buf), i);
	break;

      default:
	GXSTD::cout << "Bad table type" << "\n" << GXSTD::flush;
	return 0;
    }
  }
  return 1;
}

int CheckError(gxRDBMS *rdb)
// Function used to test the database for errors following a database 
// transaction. Returns true if an error condition is detected.
{
  if(!rdb->IsOpen()) {
    GXSTD::cout << "Database is not open" << "\n" << GXSTD::flush;
    return 1;
  }

  gxDatabaseError db_err = rdb->GetDatabase()->GetDataFileError();
  int errors = 0;
  if(db_err != gxDBASE_NO_ERROR) {
    GXSTD::cout << "RDBMS data file reported errors" << "\n" << GXSTD::flush;
    GXSTD::cout << rdb->GetDatabase()->DataFileExceptionMessage() 
		<< "\n" << GXSTD::flush; 
    errors++;
  }

  for(int i = 0; i < rdb->NumTrees(); i++) {
    gxDatabaseError ix_err = rdb->GetDatabase()->GetIndexFileError(i);
    if(ix_err != gxDBASE_NO_ERROR) {
      GXSTD::cout << "RDBMS index #" << (i+1) << " reported errors" 
		  << "\n" << GXSTD::flush;
      GXSTD::cout << rdb->GetDatabase()->IndexFileExceptionMessage(i) 
		  << "\n" << GXSTD::flush; 
      errors++;
    }
  }
  return errors != 0;
}

void PrintKeyInfo(gxRDBMS *rdb) 
// Function used to display kef definition information.
{
  GXSTD::cout << "RDBMS Key information" << "\n" << GXSTD::flush;
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Number of B-tree index files = " << rdb->NumTrees() 
	      << "\n" << GXSTD::flush;
  GXSTD::cout << "B-tree node order = " << rdb->Order() << "\n" 
	      << GXSTD::flush;
  GXSTD::cout << "Comparing string case = ";
  rdb->NoCaseCompare() == 1 ? GXSTD::cout << "No" : GXSTD::cout << "Yes";
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Allowing duplicate entries = ";
  rdb->AllowDup() == 1 ? GXSTD::cout << "Yes" : GXSTD::cout << "No";
  GXSTD::cout << "\n" << GXSTD::flush;
  if(rdb->PrimaryKey() == -1) {
    GXSTD::cout << "No primary keys defined" << "\n" << GXSTD::flush;
    GXSTD::cout << "The default key will use all record fields" << "\n" 
		<< GXSTD::flush;
    return;
  }
  GXSTD::cout << "Primary Key = field #" << rdb->PrimaryKey() << "\n" 
	      << GXSTD::flush;
  if(rdb->NumSecKeys() > 0) {
    GXSTD::cout << "Number of secondary keys = " << rdb->NumSecKeys() << "\n" 
		<< GXSTD::flush;
    GXSTD::cout << "Secondary key fields = ";
    for(int i = 0; i < rdb->NumSecKeys(); i++) {
      GXSTD::cout << rdb->SecKeyFields()[i] << " ";
    }
    GXSTD::cout << "\n" << GXSTD::flush;
  }
  GXSTD::cout << "\n" << GXSTD::flush;
}

void PrintBtreeHeader(gxBtreeHeader &hdr)
// Function used to display a gxBtree header.
{
  GXSTD::cout << "Node order:      " << hdr.node_order << "\n" << GXSTD::flush;
  GXSTD::cout << "Key size:        " << hdr.key_size << "\n" << GXSTD::flush;
  GXSTD::cout << "Number of keys:  " << hdr.n_keys << "\n" << GXSTD::flush;
  GXSTD::cout << "Number of nodes: " << hdr.n_nodes << "\n" << GXSTD::flush;
  GXSTD::cout << "B-tree height:   " << hdr.btree_height << "\n" 
	      << GXSTD::flush;
  GXSTD::cout << "Root Address:    " << (long)hdr.root << "\n" << GXSTD::flush;
  // NOTE: The root node FAU is cast to a long for compilers
  // without native 64-bit int ostream overloads
  GXSTD::cout << "Number of trees: " << hdr.num_trees << "\n" << GXSTD::flush;
}

void PrintBtreeHeader(gxRDBMS *rdb)
// Function used to display all gxBtree headers.
{
  PrintKeyInfo(rdb);
  gxBtreeHeader hdr;
  for(int i = 0; i < rdb->NumTrees(); i++) {
    GXSTD::cout << "B-tree #" << (i+1) << " header information" << "\n" 
		<< GXSTD::flush;
    rdb->GetDatabase()->Index(i)->ReadBtreeHeader(hdr);
    PrintBtreeHeader(hdr);
    PausePrg();
  }
}

void DeleteRecord(gxRDBMS *rdb)
// Function used to delete a record from the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Deleting specified entry..." << "\n" << GXSTD::flush;

  if(rdb->IsEmpty()) {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    return;
  }
  gxrdDatabaseRecord record(rdb->RecordDef());

  InputRecordData(rdb, record);
  if(!rdb->DeleteRecord(&record)) {
    GXSTD::cout << "Could not find database record" << "\n" << GXSTD::flush;
    return;
  }

  GXSTD::cout << "Deleted database record" << "\n" << GXSTD::flush;

  rdb->Flush(); // Flushing the database buffers following manual deletion
}

void DumpDatabase(gxRDBMS *rdb, unsigned index_number)
// Dump the contents of the database using the index file.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Indexed database listing..." << "\n" << GXSTD::flush;

  if(rdb->IsEmpty()) {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    return;
  }

  gxrdDatabaseRecord record(rdb->RecordDef());
  gxrdDatabaseKey key, compare_key;
  gxBtree *btx = rdb->GetBtree(index_number);
  BtreeNode curr_node(btx->KeySize(), btx->NodeOrder());

  // Walk through the tree starting at the first key
  if(btx->FindFirst(key)) {
    if(!rdb->ReadRecord(&record, key.KeyID())) {
      GXSTD::cout << "Error reading the database record" << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report all errors
      return;
    }
    DisplayRecord(rdb, record);
    
    while(btx->FindNext(key, compare_key, curr_node)) {
      if(!rdb->ReadRecord(&record, key.KeyID())) {
	GXSTD::cout << "Error reading the database record" << "\n" 
		    << GXSTD::flush;
	CheckError(rdb); // Report all errors
	return;
      }
      DisplayRecord(rdb, record);
    }
  }
}

int LoadTestRecord(gxRDBMS *rdb, gxrdDatabaseRecord &record,
		   __ULWORD__ i)
// Function used for testing only. Loads data into a test record.
// Returns true if the function is loaded with no errors.
{
  __ULWORD__ j, length;
  int num_strings = 1;
  int type;

  // Field type buffers
  gxString sbuf;
  gxINT16 i16buf = (gxINT16)i;
  gxINT32 i32buf = (gxINT32)i;
  gxFLOAT64 f64buf = (gxFLOAT64)(i + .95);
  CDate date_buf((char)i, (char)i, (short)i);
  char bool_buf = 'Y';

#if defined (__64_BIT_DATABASE_ENGINE__)
  gxINT64 i64buf = (gxINT64)i;
#else
  gxINT32 low_word = (gxINT32)i;
  gxINT32 high_word = (gxINT32)i;
  char *ptr;
#endif

  record.Clear();
  record.CreateRecord(rdb->RecordDef());

  for(j = 0; j < rdb->Table().table_header.num_fields; j++) {
    type = record.FieldType(j);
    switch(type) {
      case gxrdBOOL :
	record.SetField(&bool_buf, sizeof(bool_buf), j);
	break;

      case gxrdCHAR : case gxrdVCHAR :
	sbuf << clear << "Record-" << (i+num_strings++); 
	length = sbuf.length();
	if(type == gxrdCHAR) {
	  // Ensure fixed length strings stay inbounds
	  if(length > record.FieldLength(j)) length = record.FieldLength(j);
	}
	record.SetField(sbuf.GetSPtr(), length, j);
	break;

      case gxrdBINARY : 
	sbuf << clear << "UserDefinedSpace-" << (i+num_strings++);
	record.SetField(sbuf.GetSPtr(), sbuf.length(), j);
	break;

      case gxrdBLOB :
	sbuf << clear << "BLOBSpace-" << (i+num_strings++);
	record.SetField(sbuf.GetSPtr(), sbuf.length(), j);
	break;

      case gxrdINT16 :
	i16buf++;
	record.SetField(&i16buf, sizeof(i16buf), j);
	break;

      case gxrdINT32 :
	i32buf++;
	record.SetField(&i32buf, sizeof(i32buf), j);
	break;
	
      case gxrdINT64 :
#if defined (__64_BIT_DATABASE_ENGINE__)
	i64buf++;
	record.SetField(&i64buf, sizeof(i64buf), j);
#else
	low_word++; high_word++;
	ptr = (char *)record.FieldData(j);
	memmove(ptr, &low_word, sizeof(low_word));
	memmove((ptr+sizeof(low_word)), &high_word, sizeof(high_word));
#endif
	break;

      case gxrdFLOAT : case gxrdCURRENCY :
	f64buf++;
	record.SetField(&f64buf, sizeof(f64buf), j);
	break;

      case gxrdDATE :
	record.SetField(&date_buf, sizeof(date_buf), j);
	break;
	
      default:
	GXSTD::cout << "Bad field type" << "\n" << GXSTD::flush;
	return 0;
    }
  }
  return 1;
}

void BatchDelete(gxRDBMS *rdb, unsigned index_number)
// Function used to delete all the database records
// using a batch delete.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Deleting all database entries..." << "\n" << GXSTD::flush;
  
  if(rdb->IsEmpty()) {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    return;
  }

  gxrdDatabaseRecord record(rdb->RecordDef());
  gxrdDatabaseKey key, compare_key;
  gxBtree *btx = rdb->GetBtree(index_number);

  __ULWORD__ record_count = 0;
  double delete_time = 0;
  clock_t begin, end, begin_delete, end_delete;

  // Walk through the tree starting at the first key
  begin = clock();
  while(btx->FindFirst(key)) {
    if(!rdb->ReadRecord(&record, key.KeyID())) {
      GXSTD::cout << "Error reading the database record" << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report all errors
      return;
    }
    begin_delete = clock();
    if(!rdb->DeleteRecord(&record)) {
      GXSTD::cout << "Error deleting record" << "\n" << GXSTD::flush;
      CheckError(rdb); // Report data and index file records
      rdb->Flush();
      return;
    }
    end_delete = clock();
    delete_time += (double)(end_delete - begin_delete) / CLOCKS_PER_SEC;
    record_count++;
  }
  end =clock();
  
  GXSTD::cout.precision(3);
  double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  GXSTD::cout << "Deleted " << record_count << " records in " 
	      << elapsed_time << " seconds" << "\n" << GXSTD::flush;
  double avg_delete_time = (delete_time/(double)record_count) * 1000;
  GXSTD::cout << "Average delete time = " << avg_delete_time << " milliseconds"
	      << "\n" << GXSTD::flush;

  // Flush the database files after a batch delete
  rdb->Flush();
}


void BatchInsert(gxRDBMS *rdb)
// Function used to build a large database using a batch insert
// and test the database using a batch delete followed by another 
// batch insert.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  gxrdDatabaseRecord record(rdb->RecordDef());
  __ULWORD__ num, i, j;
  unsigned long record_count = 0;
  unsigned long curr_count = 0;
  int verify_deletions = 0;
  int reinsert = 1;

  GXSTD::cout << "Enter the number of entries to add: ";
  GXSTD::cin >> num;
  if(!GXSTD::cin) {
    GXSTD::cout << "Invalid entry. No objects added!" << "\n" << GXSTD::flush;
    return;
  }

  GXSTD::cout << "Adding " << num << " records to the database..." 
	      << "\n" << GXSTD::flush;

  double insert_time = 0;
  
  // Get CPU clock cycles before entering loop
  clock_t begin = clock();

  for(i = 0; i < num; i++) {
    if(!LoadTestRecord(rdb, record, i)) return;
    clock_t begin_insert = clock();
    if(!rdb->AddRecord(&record)) {
      GXSTD::cout << "Error adding record #" << (i+1) << "\n" << GXSTD::flush;
      CheckError(rdb); // Report data and index file records
      rdb->Flush();
      return;
    }
    clock_t end_insert = clock();
    insert_time += (double)(end_insert - begin_insert) / CLOCKS_PER_SEC;
    record_count++;
    curr_count++;
    if(curr_count == (num/10)) {
      curr_count = 0;
      GXSTD::cout << "Inserted " << (i+1) << " records in " << insert_time
		  << " seconds" << "\n" << GXSTD::flush;
    }
  }
  // Get CPU clock cycles after loop is completed 
  clock_t end =clock();

  // Calculate the elapsed time in seconds. 
  double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  GXSTD::cout.precision(3); 
  GXSTD::cout << "Inserted " << record_count << " records in " 
	      << elapsed_time << " seconds" << "\n" << GXSTD::flush;
  double avg_insert_time = (insert_time/(double)record_count) * 1000;
  GXSTD::cout << "Average insert time = " << avg_insert_time 
	      << " milliseconds" << "\n" << GXSTD::flush; 

  // Flush the database files after a batch insert
  rdb->Flush();

  record_count = 0;
  double search_time = 0;
  GXSTD::cout << "Verifying the insertions..." << "\n" << GXSTD::flush;
  begin = clock();
  for(i = 0; i < num; i++) {
    if(!LoadTestRecord(rdb, record, i)) return;
    clock_t begin_search = clock();
    if(!rdb->FindRecord(&record)) {
      GXSTD::cout << "Error searching for record #" << (i+1) << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report data and index file records
      rdb->Flush();
      return;
    }
    clock_t end_search = clock();
    record_count++;
    search_time += (double)(end_search - begin_search) / CLOCKS_PER_SEC;
  }

  end =clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  GXSTD::cout.precision(3);
  GXSTD::cout << "Verified " << record_count << " records in " 
	      << elapsed_time << " seconds" << "\n" << GXSTD::flush;
  double avg_search_time = (search_time/(double)record_count) * 1000;
  GXSTD::cout << "Average search time = " << avg_search_time << " milliseconds"
	      << "\n" << GXSTD::flush;

  GXSTD::cout << "Deleting all the entries..." << "\n" << GXSTD::flush;
  record_count = 0;
  begin = clock();
  double delete_time = 0;
  for(i = 0; i < num; i++) {
    if(!LoadTestRecord(rdb, record, i)) return;
    clock_t begin_delete = clock();
    if(!rdb->DeleteRecord(&record)) {
      GXSTD::cout << "Error deleting record #" << (i+1) << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report data and index file records
      rdb->Flush();
      return;
    }
    clock_t end_delete = clock();
    delete_time += (double)(end_delete - begin_delete) / CLOCKS_PER_SEC;
    record_count++;
    if(verify_deletions) { // Verify the remaining key locations
      for(j = num-1; j != i; j--) {
	gxrdDatabaseRecord record2(rdb->RecordDef());
	if(!LoadTestRecord(rdb, record2, j)) return;
	if(!rdb->FindRecord(&record2)) {
	  GXSTD::cout << "Error finding record  - " << (j+1) << "\n" 
		      << GXSTD::flush;
	  GXSTD::cout << "After deleting record - " << (i+1) << "\n" 
		      << GXSTD::flush;
	  CheckError(rdb); // Report data and index file records
	  rdb->Flush();
	  return;
	}
      }
    }
  }

  end =clock();
  GXSTD::cout.precision(3);
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  GXSTD::cout << "Deleted " << record_count << " records in " 
	      << elapsed_time << " seconds" << "\n" << GXSTD::flush;
  double avg_delete_time = (delete_time/(double)record_count) * 1000;
  GXSTD::cout << "Average delete time = " << avg_delete_time << " milliseconds"
	      << "\n" << GXSTD::flush;

  // Flush the database files after a batch delete
  rdb->Flush();

  if(reinsert) {
    record_count = 0;
    insert_time = 0;
    GXSTD::cout << "Re-inserting " << num << " keys..." << "\n" 
		<< GXSTD::flush;
    begin = clock();
    for(i = 0; i < num; i++) {
      if(!LoadTestRecord(rdb, record, i)) return;
      clock_t begin_insert = clock();
      if(!rdb->AddRecord(&record)) {
	GXSTD::cout << "Error adding record #" << i << "\n" << GXSTD::flush;
	CheckError(rdb); // Report data and index file records
	rdb->Flush();
	return;
      }
      clock_t end_insert = clock();
      insert_time += (double)(end_insert - begin_insert) / CLOCKS_PER_SEC;
      record_count++;
      curr_count++;
    }
    end =clock();
    elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
    GXSTD::cout.precision(3); 
    GXSTD::cout << "Inserted " << record_count << " records in " 
		<< elapsed_time << " seconds" << "\n" << GXSTD::flush;
    avg_insert_time = (insert_time/(double)record_count) * 1000;
    GXSTD::cout << "Average insert time = " << avg_insert_time 
		<< " milliseconds" << "\n" << GXSTD::flush; 
    // Flush the database files after a batch insert
    rdb->Flush();
  }
}

void FindRecord(gxRDBMS *rdb)
// Function used to find a record in the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Searching for specified entry..." << "\n" << GXSTD::flush;

  if(rdb->IsEmpty()) {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    return;
  }

  gxrdDatabaseRecord record(rdb->RecordDef());

  InputRecordData(rdb, record);

  int load_record = 1;
  int exists = rdb->FindRecord(&record, load_record);
  if(exists) {
    GXSTD::cout << "Found database record" << "\n" << GXSTD::flush;
    DisplayRecord(rdb, record);
  }
  else {
    GXSTD::cout << "Did not find database record" << "\n" << GXSTD::flush;
  }
  CheckError(rdb); // Report all errors
}

void DumpDatabase(gxRDBMS *rdb)
// Dump contents of the database without using the index file.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Linear database listing..." << "\n" << GXSTD::flush;

  if(rdb->IsEmpty()) {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    return;
  }

  gxrdDatabaseRecord record(rdb->RecordDef());
  gxDatabase *f = rdb->GetDatabase()->OpenDataFile();
  gxBlockHeader blk;
  FAU_t block_address = (FAU_t)0;
  unsigned count = 0;
  ClearInputStream(GXSTD::cin); // Clear input stream
  while(1) {
    block_address = f->FindFirstBlock(block_address);
    if(block_address == FAU_t(0)) break;
    if(rdb->ReadBlockHeader(blk, block_address) != gxDBASE_NO_ERROR) {
      GXSTD::cout << "Error reading database block" << "\n" << GXSTD::flush;
      CheckError(rdb); // Report all errors
      break;
    }
    // Process normal blocks and skip deleted or removed blocks
    if(__SBYTE__(blk.block_status & 0xFF) == gxNormalBlock) {
      if(rdb->Read(&record, blk) != gxDBASE_NO_ERROR) {
	GXSTD::cout << "Error reading database record" << "\n" << GXSTD::flush;
	CheckError(rdb); // Report all errors
	break;
      }
      DisplayRecord(rdb, record);
      count++;
      if(count == 2) {
	PausePrg();
	count = 0;
      }
    }
    block_address += blk.block_length;
  }
}

void AddRecord(gxRDBMS *rdb)
// Function used to add a record to the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Adding specified entry..." << "\n" << GXSTD::flush;

  gxrdDatabaseRecord record(rdb->RecordDef());

  InputRecordData(rdb, record);

  if(!rdb->AddRecord(&record)) {
    GXSTD::cout << "Could not add record to the database" << "\n" 
		<< GXSTD::flush;
    CheckError(rdb); // Report data and index file records
    return;
  }

  rdb->Flush(); // Flushing the database buffers following manual insertion
}

void ChangeRecord(gxRDBMS *rdb)
// Function used to change a record to the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Changing specified entry..." << "\n" << GXSTD::flush;

  gxrdDatabaseRecord from(rdb->RecordDef());
  gxrdDatabaseRecord to(rdb->RecordDef());

  GXSTD::cout << "Existing record infomation" << "\n" << GXSTD::flush;
  InputRecordData(rdb, from);

  GXSTD::cout << "New record infomation" << "\n" << GXSTD::flush;
  InputRecordData(rdb, to);

  if(!rdb->ChangeRecord(&from, &to)) {
    GXSTD::cout << "Could not change database record" << "\n" << GXSTD::flush;
    CheckError(rdb); // Report data and index file records
    return;
  }

  rdb->Flush(); // Flushing the database buffers following manual insertion
}

void ChangeOrAdd(gxRDBMS *rdb)
// Function used to change or add a record to the database.
{
  // Ensure that the in memory buffers and the file data
  // stay in sync during multiple file access.
  rdb->TestDatabase();

  GXSTD::cout << "Changing or adding specified entry..." << "\n" 
	      << GXSTD::flush;

  gxrdDatabaseRecord from(rdb->RecordDef());
  gxrdDatabaseRecord to(rdb->RecordDef());

  GXSTD::cout << "Existing record infomation" << "\n" << GXSTD::flush;
  InputRecordData(rdb, from);

  GXSTD::cout << "New record infomation" << "\n" << GXSTD::flush;
  InputRecordData(rdb, to);

  if(!rdb->ChangeOrAdd(&from, &to)) {
    GXSTD::cout << "Could not change database record" << "\n" << GXSTD::flush;
    CheckError(rdb); // Report data and index file records
    return;
  }

  rdb->Flush(); // Flushing the database buffers following manual insertion
}

void CheckIndex(gxRDBMS *rdb)
// Function used to manually check the indexing subsystem.
{
  if(!rdb->UsingIndex()) {
    GXSTD::cout << "This database is not using the indexing sub-system" 
		<< "\n" << GXSTD::flush;
    return;
  }
  GXSTD::cout << "Checking the indexing sub-system" << "\n" << GXSTD::flush;

  gxString mesg, sbuf;
  FAU_t keys;
  int rebuild;
  gxDatabaseError err;

  for(int i = 0; i < rdb->NumTrees(); i++) {
    GXSTD::cout << "Testing index #" << (i+1) << "\n" << GXSTD::flush;

    err = rdb->CheckIndex(i, rebuild, &mesg); 
    if(err != gxDBASE_NO_ERROR) {
      GXSTD::cout << "Index #" << (i+1) << " reported errors" << "\n" 
		  << GXSTD::flush;
      GXSTD::cout << mesg;
      CheckError(rdb); // Report all errors
      if(rebuild) {
	GXSTD::cout << "Index #" << (i+1) << " needs to be rebuilt" << "\n" 
		    << GXSTD::flush;
      }
    }
    if(!rdb->VerifyIndex(i, keys)) {
      GXSTD::cout << "Index #" << (i+1) << " reported errors" << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report all errors
      GXSTD::cout << "Index #" << (i+1) << " needs to be rebuilt" << "\n" 
		  << GXSTD::flush;
      PausePrg();
      return;
    }
    if(keys) {
      GXSTD::cout << "Index #" << (i+1) << " checks tests good" << "\n" 
		  << GXSTD::flush;
      sbuf << clear << keys;
      GXSTD::cout << "Tested " << sbuf.c_str() << " entry keys" << "\n" 
		  << GXSTD::flush;
    }
    else {
      GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
    }
    PausePrg();
  }
}

void RebuildIndex(gxRDBMS *rdb)
// Function used to manually rebuild the indexing subsystem.
{
  if(!rdb->UsingIndex()) {
    GXSTD::cout << "This database is not using the indexing sub-system" 
		<< "\n" << GXSTD::flush;
    return;
  }

  FAU_t records, keys;
  gxString sbuf;

  GXSTD::cout << "Rebuilding the indexing sub-system" << "\n" << GXSTD::flush;
  for(int i = 0; i < rdb->NumTrees(); i++) {
    GXSTD::cout << "Rebuilding index #" << (i+1) << "\n" << GXSTD::flush;
    if(!rdb->RebuildIndex(i, records, keys)) {
      GXSTD::cout << "Index #" << (i+1) << " reported errors" << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report all errors
      GXSTD::cout << "Index #" << (i+1) << " rebuild failed" << "\n" 
		  << GXSTD::flush;
      PausePrg();
      return;
    }
    GXSTD::cout << "Index #" << (i+1) << " rebuild" << "\n" << GXSTD::flush;
    sbuf << clear << records;
    GXSTD::cout << "Read " << sbuf.c_str() << " data file records" << "\n" 
		<< GXSTD::flush;
    sbuf << clear << keys;
    GXSTD::cout << "Added " << sbuf.c_str() << " index keys" << "\n" 
		<< GXSTD::flush;
    PausePrg();
  }
}

void CheckDataFile(gxRDBMS *rdb)
// Function used to manually check the data file.
{
  GXSTD::cout << "Checking the data file" << "\n" << GXSTD::flush;

  FAU_t records;
  gxString mesg, sbuf;
  int rebuild_df, rebuild_ix;
  gxDatabaseError err;
  
  err = rdb->CheckDataFile(rebuild_df, rebuild_ix, &mesg); 
  if(err != gxDBASE_NO_ERROR) {
    GXSTD::cout << "Data file reported errors" << "\n" << GXSTD::flush;
    GXSTD::cout << mesg;
    CheckError(rdb); // Report all error
    if(rebuild_df) {
      GXSTD::cout << "The datafile needs to be rebuilt" << "\n" 
		  << GXSTD::flush;
    }
    if(rebuild_ix) {
      GXSTD::cout << "The indexing subsystem needs to be rebuilt" << "\n" 
		  << GXSTD::flush;
    }
  }
  GXSTD::cout << "Verifying all data file records" << "\n" << GXSTD::flush;
  if(!rdb->VerifyDataFile(records)) {
    GXSTD::cout << "Data file reported errors" << "\n" << GXSTD::flush;
    CheckError(rdb); // Report all errors
  }
  if(records) {
      GXSTD::cout << "Data file checks tests good" << "\n" << GXSTD::flush;
      sbuf << clear << records;
      GXSTD::cout << "Tested " << sbuf.c_str() << " records" << "\n" 
		  << GXSTD::flush;
    }
  else {
    GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
  }
}

void RebuildDataFile(gxRDBMS *rdb)
// Function used to manually rebuild the data file.
{
  FAU_t records;
  gxString sbuf;

  if(!rdb->RebuildDataFile(records)) {
    GXSTD::cout << "Error rebuilding data file" << "\n" << GXSTD::flush;
    CheckError(rdb); // Report all errors
    return;
  }
  GXSTD::cout << "Data file rebuilt with no errors" << "\n" << GXSTD::flush;
  sbuf << clear << records;
  GXSTD::cout << "Added " << sbuf.c_str() << " records" << "\n" 
	      << GXSTD::flush;
  
  // Must rebuild all associated index files after rebuilding the 
  // data file.
  RebuildIndex(rdb);
}

int DatabaseAutoRecover(gxRDBMS *rdb, int recover_datafile, int indexnum) 
// Function used to automatically recover the database files. Returns
// true if the database is recovered with no error.
{
  int i;
  FAU_t records1, records2, keys;
  gxString sbuf;
  if(recover_datafile) {
    GXSTD::cout << "Entering automatic recovery mode" << "\n" << GXSTD::flush;
    GXSTD::cout << "Rebuilding data file" << "\n" << GXSTD::flush;
    if(!rdb->RebuildDataFile(records1)) {
      GXSTD::cout << "Error rebuilding data file" << "\n" << GXSTD::flush;
      CheckError(rdb); // Report all errors
      return 0;
    }
    GXSTD::cout << "Data file rebuilt with no errors" << "\n" << GXSTD::flush;
    sbuf << clear << records1;
    GXSTD::cout << "Added " << sbuf.c_str() << " records during rebuild" 
		<< "\n" << GXSTD::flush;
    GXSTD::cout << "Running data file consistency check" << "\n" 
		<< GXSTD::flush;
    if(!rdb->VerifyDataFile(records2)) {
      GXSTD::cout << "Data file consistency check reported errors" 
		  << "\n" << GXSTD::flush;
      CheckError(rdb); // Report all errors
      return 0;
    }
    if(records1 != records2) {
      if(records2 == (FAU_t)0) {
	GXSTD::cout << "All records were lost during consistency check" 
		    << "\n" << GXSTD::flush;
	GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
      }
      else {
	FAU_t lost_records = records1 - records2;
	if(lost_records == 1) {
	  GXSTD::cout << "Lost 1 record during consistency check" 
		      << "\n" << GXSTD::flush;
	}
	else {
	  sbuf << clear << lost_records;
	  GXSTD::cout << "Lost " << sbuf.c_str()
		      << " records during consistency check" << "\n" 
		      << GXSTD::flush;
	}
      }
    }
    GXSTD::cout << "Data file consistency check complete" << "\n" 
		<< GXSTD::flush;
    sbuf << clear << records2;
    GXSTD::cout << "Tested " << sbuf.c_str() << " records" << "\n" 
		<< GXSTD::flush;
    
    GXSTD::cout << "Rebuilding all associated index files" << "\n" 
		<< GXSTD::flush;
    for(i = 0; i < rdb->NumTrees(); i++) {
      GXSTD::cout << "Rebuilding index #" << (i+1) << "\n" << GXSTD::flush;
      if(!rdb->RebuildIndex(i, records1, keys)) {
	GXSTD::cout << "Index #" << (i+1) << " reported errors" << "\n" 
		    << GXSTD::flush;
	CheckError(rdb); // Report all errors
	GXSTD::cout << "Index #" << (i+1) << " rebuild failed" << "\n" 
		    << GXSTD::flush;
	return 0;
      }
      GXSTD::cout << "Index #" << (i+1) << " rebuilt" << "\n" << GXSTD::flush;
      sbuf << clear << records1;
      GXSTD::cout << "Read " << sbuf.c_str() << " data file records" << "\n" 
		  << GXSTD::flush;
      sbuf << clear << keys;
      GXSTD::cout << "Added " << sbuf.c_str() << " index keys" << "\n" 
		  << GXSTD::flush;
    }
  }
  else {
    GXSTD::cout << "Entering automatic recovery mode" << "\n" << GXSTD::flush;
    GXSTD::cout << "Rebuilding index #" << (indexnum+1) << "\n" 
		<< GXSTD::flush;
    if(!rdb->RebuildIndex(indexnum, records1, keys)) {
      GXSTD::cout << "Index #" << (indexnum+1) << " reported errors" << "\n" 
		  << GXSTD::flush;
      CheckError(rdb); // Report all errors
      GXSTD::cout << "Index #" << (indexnum+1) << " rebuild failed" << "\n" 
		  << GXSTD::flush;
      return 0;
    }
    GXSTD::cout << "Index #" << (indexnum+1) << " rebuilt" << "\n" 
		<< GXSTD::flush;
    sbuf << clear << records1;
    GXSTD::cout << "Read " << sbuf.c_str() << " data file records" << "\n" 
		<< GXSTD::flush;
    sbuf << clear << keys;
    GXSTD::cout << "Added " << sbuf.c_str() << " index keys" << "\n" 
		<< GXSTD::flush;
  }
  return 1;
}

int DatabaseConsistencyCheck(gxRDBMS *rdb, int auto_recover, int force_verify) 
// Function used to test the consistency of the database. Returns true if the 
// consistency check passes.
{
  gxString mesg, sbuf;
  FAU_t records, keys;
  int i, j, rebuild_df, rebuild_ix, rv;
  gxDatabaseError err;
  const int RETRY_ON_ERROR = 3;
  
  GXSTD::cout << "Checking data file..." << "\n" << GXSTD::flush;
  i = RETRY_ON_ERROR;
  rv = 0;
  while(i--) {
    err = rdb->CheckDataFile(rebuild_df, rebuild_ix, &mesg);
    if(err == gxDBASE_BAD_TABLE_FIELD) {
      GXSTD::cout << "Cannot open database due to data type mismatch" 
		  << "\n" << GXSTD::flush;
      GXSTD::cout << "The DEF field types must match the table header" 
		  << "\n" << GXSTD::flush;
      return 0;
    }
    if((rebuild_df) || (rebuild_ix)) break;
    if(err != gxDBASE_NO_ERROR) {
      GXSTD::cout << "Encountered error during data file check" 
		  << "\n" << GXSTD::flush;
      GXSTD::cout << gxDatabaseExceptionMessage(err) << "\n" << GXSTD::flush;
      GXSTD::cout << "Retrying data file check" << "\n" << GXSTD::flush;
    }
    else {
      rv = 1;
      break;
    }
  }

  if((!rebuild_df) && (rebuild_ix)) {
    GXSTD::cout << "The database key DEF has changed" << "\n" << GXSTD::flush;
    if(auto_recover) {
      GXSTD::cout << "Indexing subsystem has been scheduled to be rebuilt"
		  << "\n" << GXSTD::flush;
    }
    else {
      GXSTD::cout << "The indexing subsystem must be rebuilt" << "\n" 
		  << GXSTD::flush;
    }
    if(err == gxDBASE_NO_ERROR) rv = 1; // The data file checked good
  }

  if(!rv) {
    if(rebuild_df) {
      GXSTD::cout << "Data file reported errors and must be rebuilt" << "\n" 
		  << GXSTD::flush;
      if(err != gxDBASE_NO_ERROR) {
	GXSTD::cout << gxDatabaseExceptionMessage(err) << "\n" << GXSTD::flush;
      }
    }
    else {
      GXSTD::cout << "Data file reported errors" << "\n" << GXSTD::flush;
    }
    if(auto_recover) {
      if(!DatabaseAutoRecover(rdb, 1, 0)) return 0;
    }
  }
  else {
    if(force_verify) {
      GXSTD::cout << "Verifying data file consistency" << "\n" << GXSTD::flush;
      if(!rdb->VerifyDataFile(records)) {
	GXSTD::cout << "Data file reported errors during consistency check" 
		    << "\n" << GXSTD::flush;
	CheckError(rdb); // Report all errors
	if(auto_recover) {
	  if(!DatabaseAutoRecover(rdb, 1, 0)) return 0;
	  return 1; // The auto recovery call will rebuild the index file
	}
      }
      else {
	if(records) {
	  GXSTD::cout << "Data file checks tests good" << "\n" << GXSTD::flush;
	  sbuf << clear << records;
	  GXSTD::cout << "Tested " << sbuf.c_str() << " records" << "\n" 
		      << GXSTD::flush;
	}
	else {
	  GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
	}
      }
    }
    GXSTD::cout << "Data file checks good" << "\n" << GXSTD::flush;
    if(rebuild_ix) {
      GXSTD::cout << "Rebuilding indexing subsystem due to change in DEF file"
		  << "\n" << GXSTD::flush;
      for(i = 0; i < rdb->NumTrees(); i++) {
	if(auto_recover) {
	  if(!DatabaseAutoRecover(rdb, 0, i)) return 0;
	  continue; // Check the next index file
	}
      }
      return 1;
    }

    GXSTD::cout << "Checking associated index files..." << "\n" 
		<< GXSTD::flush;
    for(i = 0; i < rdb->NumTrees(); i++) {
      GXSTD::cout << "Checking index #" << (i+1) << "\n" << GXSTD::flush;
      j = RETRY_ON_ERROR;
      rv = 0;
      while(j--) {
	err = rdb->CheckIndex(i, rebuild_ix, &mesg);
	if(rebuild_ix) break;
	if(err != gxDBASE_NO_ERROR) {
	  GXSTD::cout << "Encountered error while checking index " 
	       << (i+1) << "\n" << GXSTD::flush;
	  GXSTD::cout << gxDatabaseExceptionMessage(err) << "\n" 
		      << GXSTD::flush;
	  GXSTD::cout << "Retrying index file check" << "\n" << GXSTD::flush;
	}
	else {
	  rv = 1;
	  break;
	}
      }
      if(!rv) {
	if(rebuild_ix) {
	  GXSTD::cout << "Index #" << (i+1) 
		      << " reported errors and must be rebuilt" 
		      << "\n" << GXSTD::flush;
	  if(err != gxDBASE_NO_ERROR) {
	    GXSTD::cout << gxDatabaseExceptionMessage(err) << "\n" 
			<< GXSTD::flush;
	  }
	}
	else {
	  GXSTD::cout << "Index #" << (i+1) << " reported errors" << "\n" 
		      << GXSTD::flush;
	  if((err == gxDBASE_BAD_INDEX_NUMBER) || 
	     (err == gxDBASE_BAD_INDEX_NAME)) {
	    GXSTD::cout << "Cannot recover from current error condition" 
			<< "\n" << GXSTD::flush;
	    return 0;
	  }
	}
	if(auto_recover) {
	  if(!DatabaseAutoRecover(rdb, 0, i)) return 0;
	  continue; // Check the next index file
	}
      }
      else {
	if(force_verify) {
	  GXSTD::cout << "Verifying index #" << (i+1) << " file consistency" 
	       << "\n" << GXSTD::flush;
	  if(!rdb->VerifyIndex(i, keys)) {
	    GXSTD::cout << "Index #" << (i+1) 
		 << " reported errors during consistency check" 
		 << "\n" << GXSTD::flush;
	    CheckError(rdb); // Report all errors
	    if(auto_recover) {
	      if(!DatabaseAutoRecover(rdb, 0, i)) return 0;
	      continue; // Check the next index file
	    }
	    else {
	      if(keys) {
		GXSTD::cout << "Index #" << (i+1) << " checks tests good" 
		     << "\n" << GXSTD::flush;
		sbuf << clear << keys;
		GXSTD::cout << "Tested " << sbuf.c_str() << " entry keys" 
			    << "\n" << GXSTD::flush;
	      }
	      else {
		GXSTD::cout << "The database is empty" << "\n" << GXSTD::flush;
	      }
	    }
	  }
	}
	GXSTD::cout << "Index #" << (i+1) << " checks good" << "\n" 
		    << GXSTD::flush;
      }
    }
  }
  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
