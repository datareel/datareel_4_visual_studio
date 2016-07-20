// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: as2ps.h 
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 12/17/1997  
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

ASCII to PostScript converter program.  
*/
// ----------------------------------------------------------- //   
#ifndef __ASPS_HPP__
#define __ASPS_HPP__

#include "gxdlcode.h"

#include "pscript.h"
#include "gxd_ver.h"

// Postscript driver version number and program name
const double ASPSVersionNumber = gxDatabaseVersion;

#if defined (__64_BIT_DATABASE_ENGINE__)
const char *ProgramName = "as2ps64";
#else // Use the 32-bit version by default
const char *ProgramName = "as2ps";
#endif

// AS2PS program functions
void HelpMessage(const char *program_name, const double version_number);
PostScriptDrv ProcessArgs(int argc, char *argv[]);
int GenOutputFileName(char *extension);
void DocumentSetup(PostScriptDrv &psdrv);

#endif  // __ASPS_HPP__
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
