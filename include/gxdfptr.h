// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: gxdfptr.h
// Compiler Used: MSVC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 02/04/1997 
// Date Last Modified: 05/03/2016
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

The gxDatabase file pointer routines are a collection of classes 
and standalone functions used to define the underlying file system
used by the gxDatabase class. NOTE: None of the data structures
and functions defined here are intended to be used directly. They
are used by the gxDatabase class to call the correct native file
API function for each supported platform.

Changes:
==============================================================
02/01/2002: Add enhanced NTFS compatibility to all WIN32 functions 
by adding WIN32 file I/O. Define the __WIN32__ and __NTFS__ 
preprocessor directives to use the WIN32 file I/O API instead of 
the stdio file I/O.  

03/18/2002: Changed the return types for the gxdFPTRRead() and
gxdFPTRWrite() functions to return the number of bytes read and
moved rather than the returning 0/1 to indicate a pass/fail 
condition. This change was made to support additional classes
throughout the library.
==============================================================
*/
// ----------------------------------------------------------- //  
#ifndef __GX_DATABASE_FILE_POINTER_HPP__
#define __GX_DATABASE_FILE_POINTER_HPP__

#include "gxdlcode.h"

// Select the underlying file system used for the database engine
#if defined (__64_BIT_DATABASE_ENGINE__)
#include "gxdfp64.h" // WIN32 will always use NTFS for large files
typedef gxdFPTR64 gxdFPTR;

#elif defined (__WIN32__) && defined (__NTFS__)
// WIN32 file I/O API
#include <windows.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "gxdtypes.h"
#include "gxheader.h"

struct GXDLCODE_API gxdFPTR { // Platform specific file pointer type
  HANDLE fptr;
};

#else // Use the stdio version by default. Common to all platforms.
// Non-platform specific stdio include files
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "gxdtypes.h"
#include "gxheader.h"

#if defined (_USE_DIRECT_IO_) && defined (__LINUX__)
struct DIOFILE {
  int fd;
  int access;
  int perm;
  char *fname;
};

#if defined USE_IOBUF_MACROS
#define POSIX_OPEN iobuf_open
#define POSIX_CREAT iobuf_creat
#define POSIX_CLOSE iobuf_close
#define POSIX_READ iobuf_read
#define POSIX_WRITE iobuf_write
#define POSIX_READV iobuf_readv
#define POSIX_WRITEV iobuf_writev
#define POSIX_PREAD iobuf_pread
#define POSIX_PWRITE iobuf_pwrite
#define POSIX_LSEEK iobuf_lseek
#define POSIX_FTRUNCATE iobuf_ftruncate
#define POSIX_DUP iobuf_dup
#define POSIX_DUP2 iobuf_dup2
#define POSIX_FSYNC iobuf_fsync
#define POSIX_FATASYNC iobuf_fdatasync
#else
#define POSIX_OPEN open
#define POSIX_CREAT creat
#define POSIX_CLOSE close
#define POSIX_READ read
#define POSIX_WRITE write
#define POSIX_READV readv
#define POSIX_WRITEV writev
#define POSIX_PREAD pread
#define POSIX_PWRITE pwrite
#define POSIX_LSEEK lseek
#define POSIX_FTRUNCATE ftruncate
#define POSIX_DUP dup
#define POSIX_DUP2 dup2
#define POSIX_FSYNC fsync
#define POSIX_FATASYNC fdatasync
#endif // USE_IOBUF_MACROS

// Open file, returns DIOFILE pointer or NULL if file open files
DIOFILE * dio_open(const char  *filename,  int  access,  int  permission);

DIOFILE * dio_create(const char  *filename);
int dio_close(DIOFILE *fp);
int dio_read(DIOFILE *fp, void *buf, int nbytes);
int dio_write(DIOFILE *fp, const void *buf, int nbytes);

// SEEK_SET  Offset is to be in absolute terms.
// SEEK_CUR  Offset relative to the current location of the pointer.
// SEEK_END  Offset relative to the end of the file.
off_t dio_seek(DIOFILE *fp, off_t offset, int whence);

// Get the current file position
off_t dio_tell(DIOFILE *fp);

// Flush file buffers and return 0 on success, -1 on fail
int dio_flush(DIOFILE *fp);
#endif // _USE_DIRECT_IO_

struct GXDLCODE_API gxdFPTR { // gxDatabase file pointer type
#if defined _USE_DIRECT_IO_
  DIOFILE *fptr;
#else
  FILE *fptr;
#endif
};
#endif // gxDatabase file system type

// NOTE: Any underlying file system used must provide the basic
// functionality defined here. 
GXDLCODE_API gxdFPTR *gxdFPTRCreate(const char *fname);
GXDLCODE_API gxdFPTR *gxdFPTROpen(const char *fname, 
				  gxDatabaseAccessMode mode);
GXDLCODE_API int gxdFPTRClose(gxdFPTR *stream);
GXDLCODE_API int gxdFPTRFlush(gxdFPTR *stream);
GXDLCODE_API __ULWORD__ gxdFPTRRead(gxdFPTR *stream, void *buf, 
				    __ULWORD__ bytes);
GXDLCODE_API __ULWORD__ gxdFPTRWrite(gxdFPTR *stream, const void *buf, 
				     __ULWORD__ bytes);
GXDLCODE_API FAU_t gxdFPTRSeek(gxdFPTR *stream, FAU_t, 
			       gxDatabaseSeekMode mode);
GXDLCODE_API FAU_t gxdFPTRTell(gxdFPTR *stream);
GXDLCODE_API int gxdFPTRExists(const char *fname);
GXDLCODE_API FAU_t gxdFPTRFileSize(const char *fname);

#endif // __GX_DATABASE_FILE_POINTER_HPP__
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
