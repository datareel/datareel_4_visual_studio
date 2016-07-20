// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: as2ps.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 12/17/1997
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

ASCII to PostScript converter program.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
#include <fstream>
#else // Use the old iostream library by default
#include <iostream.h>
#include <fstream.h>
#endif // __USE_ANSI_CPP__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include "as2ps.h"
#include "dfileb.h"
#include "futils.h"
#include "systime.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// AS2PS program globals
int VerboseMode = 0;     // Echo results (off by default)
char out_file[MAX_LINE]; // PostScript file created from text file
char time_buf[255];      // Buffer used to hold time/date string
char *open_file;         // File currently open
int write_to_file = 0;   // Write the output to a file instead of stdout
unsigned num_files = 0;  // Total number of files processed

// Setup as2ps default values
PostScriptDrv::PSFonts font_code = PostScriptDrv::COURIER; 
PostScriptDrv::PSPaperSizes paper_code = PostScriptDrv::LETTER_SIZE; 
double font_size = DEFAULT_POINT_SIZE; // Font or pitch size
double lr_margin = DEFAULT_LR_MARGIN;  // Left and Right margins
double tb_margin = DEFAULT_TB_MARGIN;  // Top and Bottom margins 
int man_page = 0;   // Format UNIX man page
int cut_lines = 1;  // Truncate lines to match the number of columns
int wrap_lines = 0; // Turn line wrap on or off
int duplex = 0;     // Enable duplex printing
int manualfeed = 0; // Print from manual feed position
int use_tray = 0;   // Use a specified paper tray
int tray_number;    // Tray number for the paper tray

void HelpMessage(const char *program_name, const double version_number)
{
  char gxuffer[255];
  sprintf(gxuffer, "%.3f", version_number);
  GXSTD::cout << "\n";
  GXSTD::cout << "ASCII to PostScript Converter version " << gxuffer << "\n";
  GXSTD::cout << "Usage: " << program_name << " [switches] infile.txt " 
	      << "\n";
  GXSTD::cout << "Switches: -?      = Display this help message." << "\n";
  GXSTD::cout << "          -c#     = Number of copies to print. Example: -c2"
	      << "\n";
  GXSTD::cout << "          -d      = Enable duplex printing if available." 
	      << "\n";
  GXSTD::cout << "          -D      = Print from manual feed position." 
	      << "\n";
  GXSTD::cout << "          -fcode  = Select a font type. Example: -fC" 
	      << "\n";
  GXSTD::cout << "          -F      = Write output to a file: infile.txt = \
infile.ps" << "\n";
  GXSTD::cout << "          -h      = Use page headers and numbers."
	      << "\n";
  GXSTD::cout << "          -H      = Use page headers and numbers with lines."
	      << "\n";
  GXSTD::cout << "          -l      = Print in landscape mode instead of \
portrait." << "\n";
  GXSTD::cout << "          -m#     = Set left and right margins. Example: \
-m1.0" << "\n";
  GXSTD::cout << "          -M#     = Set top and bottom margins. Example: \
-M1.0" << "\n";
  GXSTD::cout << "          -pcode  = Select a paper size. Example: -pLET" 
	      << "\n";
  GXSTD::cout << "          -s#     = Specify font size. Example: -s10.0" 
	      << "\n";
  GXSTD::cout << "          -t#     = Spaces between tab stops. Example: -t8"
	      << "\n";
  GXSTD::cout << "          -T      = Do not truncate lines that exceed \
margin." << "\n";
  GXSTD::cout << "          -u      = Format UNIX man pages produced by nroff."
	      << "\n";
  GXSTD::cout << "          -v      = Turn verbose mode on." << "\n";
  GXSTD::cout << "          -W      = Turn line wrap on." << "\n";
  GXSTD::cout << "          -y#     = Use a specified paper tray. Example: -y1"
	      << "\n";
  GXSTD::cout << "\n";
  GXSTD::cout << "Press enter to continue..." << "\n";
  GXSTD::cin.get();

  GXSTD::cout << "Valid font codes for -f option: " << "\n";
  GXSTD::cout << "\n";
  GXSTD::cout << "\tC - Courier, CB - Courier-Bold, CO - Courier-Oblique,"  
	      << "\n";
  GXSTD::cout << "\tCBO - Courier-BoldObligue, T - Times-Roman, TB - \
Times-Bold" << "\n";
  GXSTD::cout << "\tTI - Times-Italic, TBI - Times-BoldItalic, H - Helvetica" 
	      << "\n";
  GXSTD::cout << "\tHBO - Helvetica-BoldOblique, HO - Helvetica-Oblique, " 
	      << "\n";
  GXSTD::cout << "\tHB - Helvetica-Bold,  S - Symbol" << "\n";

  GXSTD::cout << "\n";
  GXSTD::cout << "Valid paper codes for -p option: " << "\n";
  GXSTD::cout << "\n";
  GXSTD::cout << "\tLET - Letter size  8.5 x 11 inches" << "\n";
  GXSTD::cout << "\tLEG - Legal size   8.5 x 14 inches" << "\n";
  GXSTD::cout << "\tTAB - Tabloid size 11 x 17 inches" << "\n";
  GXSTD::cout << "\tA3  - A3 size 297mm x 420mm (11.70 x 16.55 inches)" 
	      << "\n";
  GXSTD::cout << "\tA4  - A4 size 210mm x 297mm (8.27 x 11.70 inches)" 
	      << "\n";
  exit(0);
}

// Process Program argument list
PostScriptDrv ProcessArgs(int argc, char *argv[])
{
  // Establish default values
  PostScriptDrv psdrv;
  int ibuf;
  char *sbuf;
  
  // process the program's argument list
  int i;
  for(i = 1; i < argc; i++ ) {
    if(*argv[i] == '-') {
      char sw = *(argv[i] +1);
      switch(sw) {
	case '?' :
	  HelpMessage(ProgramName, ASPSVersionNumber);
	  break;

	case 'c' :
	  ibuf = atoi(&argv[i][2]);
	  if(ibuf <= 0) ibuf = 1;
	  psdrv.Copies(ibuf);
	  break;

	case 'F':
	  write_to_file = 1;
	  break;
	  
	case 'l' :
	  psdrv.LandScapeMode();
	  break;

	case 'm' :
	  lr_margin = atof(&argv[i][2]);
	  if(lr_margin <= 0)
	    lr_margin = DEFAULT_LR_MARGIN;
	  psdrv.UseLRMargin();
	  break;

	case 'M' :
	  tb_margin = atof(&argv[i][2]);
	  if(tb_margin <= 0)
	    tb_margin = DEFAULT_TB_MARGIN;
	  psdrv.UseTBMargin();
	  break;

	case 's' : 
	  font_size = atof(&argv[i][2]);
	  if(font_size <= 0)
	    font_size = DEFAULT_POINT_SIZE;
	  break;
	
	case 't' :
	  ibuf = atoi(&argv[i][2]);
	  if(ibuf < 1) ibuf = DEFAULT_TAB_SIZE;
	  psdrv.SetTabStop(ibuf);
	  break;

	case 'v' :
	  VerboseMode = 1;
	  break;

	case 'f' :
	  sbuf = &argv[i][2]; 
	  if(strcmp(sbuf, "C") == 0) font_code = PostScriptDrv::COURIER;
	  if(strcmp(sbuf, "CB") == 0) font_code = PostScriptDrv::COURIER_BOLD;
	  if(strcmp(sbuf, "CO") == 0)
	    font_code = PostScriptDrv::COURIER_OBLIQUE;
	  if(strcmp(sbuf, "CBO") == 0)
	    font_code = PostScriptDrv::COURIER_BOLD_OBLIQUE;

	  if(strcmp(sbuf, "T") == 0) font_code = PostScriptDrv::TIMES;
	  if(strcmp(sbuf, "TB") == 0) font_code = PostScriptDrv::TIMES_BOLD;
	  if(strcmp(sbuf, "TI") == 0) font_code = PostScriptDrv::TIMES_ITALIC;
	  if(strcmp(sbuf, "TBI") == 0)
	    font_code = PostScriptDrv::TIMES_BOLD_ITALIC;

	  if(strcmp(sbuf, "H") == 0) font_code = PostScriptDrv::HELV;
	  if(strcmp(sbuf, "HB") == 0) font_code = PostScriptDrv::HELV_BOLD;
	  if(strcmp(sbuf, "HO") == 0) font_code = PostScriptDrv::HELV_OBLIQUE;
	  if(strcmp(sbuf, "HBO") == 0)
	    font_code = PostScriptDrv::HELV_BOLD_OBLIQUE;

	  if(strcmp(sbuf, "S") == 0) font_code = PostScriptDrv::SYMBOL;
	  break;
	  
	case 'p' :
	  sbuf = &argv[i][2]; 
	  if(strcmp(sbuf, "LET") == 0)
	    paper_code = PostScriptDrv::LETTER_SIZE;
	  if(strcmp(sbuf, "LEG") == 0)
	    paper_code = PostScriptDrv::LEGAL_SIZE;
	  if(strcmp(sbuf, "TAB") == 0)
	    paper_code = PostScriptDrv::TABLOID_SIZE;
	  if(strcmp(sbuf, "A3") == 0)
	    paper_code = PostScriptDrv::A3_SIZE;
	  if(strcmp(sbuf, "A4") == 0)
	    paper_code = PostScriptDrv::A4_SIZE;
	  break;
	  
	case 'u' :
	  man_page = 1;
	  cut_lines = 0;
	  wrap_lines = 0;
	  break;

	case 'T' :
	  cut_lines = 0;
	  break;

	case 'W' :
	  wrap_lines = 1;
	  cut_lines = 0;
	  break;
	  
	case 'd' :
	  duplex = 1;
	  break;

	case 'D' :
	  manualfeed = 1;
	  break;

	case 'y' :
	  use_tray = 1;
	  tray_number = atoi(&argv[i][2]);
	  break;

	case 'h' :
	  psdrv.UseHeader();
	  break;
	  
	case 'H' :
	  psdrv.UseHeader();
	  psdrv.DrawHeaderLine();
	  break;
	  
	default:
	  GXSTD::cerr << "\n";
	  GXSTD::cerr << "Unknown switch " << argv[i] << "\n";
	  GXSTD::cerr << "Exiting..." << "\n";
	  GXSTD::cerr << "\n";
	  exit(0);
	  break;
      }
    }
  }

  return psdrv;
}

int GenOutputFileName(char *extension)
// Generate a name for the output file using the open_file
// name with the specified dot extension.
{
  int i = 0;
  for(i = 0; i < MAX_LINE; i++) out_file[i] = '\0';
  char *p = open_file;
  int len = strlen(p);
  for(i = 0; i < len && i != MAX_LINE; i++, p++) {
    if(*p == '.') break;
    out_file[i] = *p;
  }
  if((strlen(out_file) + strlen(extension)) > (MAX_LINE - 1)) return 0;
  strcat(out_file, extension); // Add the file extension (.xxx)
  return 1;
}

void DocumentSetup(PostScriptDrv &psdrv)
{
  psdrv.SetPaperSize(paper_code);
  psdrv.SetFont(font_code, font_size);
  if(psdrv.UsingHeader()) {
    psdrv.SetDocumentName(open_file);
    SysTime date;
    strcpy(time_buf, date.GetSystemDateTime());
    psdrv.SetDateString(time_buf);
  }
  
  psdrv.DocumentSetup(lr_margin, tb_margin, man_page);
  
  if(VerboseMode) {
    GXSTD::cout << "Font            = " << psdrv.TextFont() << "\n";
    GXSTD::cout << "Font Size       = " << psdrv.FontSize() << "\n";
    GXSTD::cout << "Character width = " << psdrv.CharWidth() << "\n";
    GXSTD::cout << "Chars Per inch  = " << psdrv.CharsPerInch() << "\n";
    GXSTD::cout << "Columns         = " << psdrv.Columns() << "\n";
    
    GXSTD::cout << "Paper size      = ";
    if(paper_code == PostScriptDrv::LETTER_SIZE) GXSTD::cout << "LETTER" 
							     << "\n";
    if(paper_code == PostScriptDrv::LEGAL_SIZE) GXSTD::cout << "LEGAL" << "\n";
    if(paper_code == PostScriptDrv::TABLOID_SIZE) GXSTD::cout << "TABLOID" 
							      << "\n";
    if(paper_code == PostScriptDrv::A3_SIZE) GXSTD::cout << "A3" << "\n";
    if(paper_code == PostScriptDrv::A4_SIZE) GXSTD::cout << "A4" << "\n";
    
    GXSTD::cout << "Page width      = " << psdrv.PageWidth() << "\n";
    GXSTD::cout << "Page height     = " << psdrv.PageHeight() << "\n";
    GXSTD::cout << "Starting X pos  = " << psdrv.StartX() << "\n";
    GXSTD::cout << "Starting Y pos  = " << psdrv.StartY() << "\n";
    GXSTD::cout << "Lines per page  = " << psdrv.LinesPerPage() << "\n";
    
    GXSTD::cout << "Copies          = " << psdrv.NCopies() << "\n";    
    GXSTD::cout << "Tab Stop Spaces = " << psdrv.TabStop() << "\n";
   
    GXSTD::cout << "Input text file = " << open_file << "\n";
    if(write_to_file == 0 ) { 
      GXSTD::cout << "Output PS file  = stdout" << "\n";
    }
    else {
      GenOutputFileName(".ps");
      GXSTD::cout << "Output PS file  = " << out_file << "\n";
    }
    if(psdrv.GetMode()) GXSTD::cout << "Printing in landscape mode" << "\n";
    if(psdrv.UsingLRMargin()) GXSTD::cout << "Using left and right margins" 
					  << "\n";
    if(psdrv.UsingTBMargin()) GXSTD::cout << "Using top and bottom margins" 
					  << "\n";
    GXSTD::cout << "\n";
    GXSTD::cout << "Press enter to continue..." << "\n";
    GXSTD::cin.get();
  }
}

int main(int argc, char *argv[])
// NOTE: None of the MSVC compilers will expand wildcard characters
// used in command-line arguments unless linked with the setargv.obj
// library. All the UNIX compliers will expand wildcard characters
// by default.
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  // If no argument is given print usage message to the screen 1
  if(argc < 2) {
    HelpMessage(ProgramName, ASPSVersionNumber);
    return(0);
  }

  // Process command ling arguments and files 
  int narg;
  PostScriptDrv psdrv;
  char *arg = argv[narg = 1];
  while (narg < argc) {
    if (arg[0] != '\0') {
      if (arg[0] == '-') { // Look for command line arguments
	psdrv = ProcessArgs(argc, argv);
      }
      else { 
	if(futils_isdirectory((const char *)arg)) {
	  // Do not process directories
	  arg = argv[++narg];
	  continue;
	}
	open_file = arg; // Update the open file name pointer

	DiskFileB infile(open_file);
	if(!infile) {
	  GXSTD::cerr << "\n";
	  GXSTD::cerr << "Cannot open file: " << open_file << "\n";
	  GXSTD::cerr << "Exiting..." << "\n";
	  GXSTD::cerr << "\n";
	  return 0;
	}
	num_files++;

	// Process the test file
	DocumentSetup(psdrv);
	if(write_to_file == 0 ) { 
	  psdrv.Prologue(GXSTD::cout);
	  psdrv.MediaSetup(GXSTD::cout, duplex, manualfeed, 
			   use_tray, tray_number);
	  
	  if(psdrv.ConvertTextFile(infile, GXSTD::cout, wrap_lines, 
				   cut_lines) == 0) {
	    GXSTD::cerr << "\n";
	    GXSTD::cerr << "Encountered fatal error during conversion" << "\n";
	    GXSTD::cerr << "Exiting..." << "\n";
	    GXSTD::cerr << "\n";
	    return 1;
	  }
	  psdrv.Epilogue(GXSTD::cout, psdrv.page_count);
	}
	else {
	  GenOutputFileName(".ps");
	  // Using the ostream classes allows the data to be written
	  // to a file, the stdout, or stderr.
	  GXSTD::ofstream stream(out_file, GXSTD::ios::out|GXSTD::ios::trunc);
	  if(!stream) {
	    GXSTD::cerr << "\n";
	    GXSTD::cerr << "Cannot write to " << out_file << "\n";
	    GXSTD::cerr << "\n";
	    return 1;
	  }
	  
	  psdrv.Prologue(stream);
	  psdrv.MediaSetup(stream, duplex, manualfeed, use_tray, tray_number);
	  
	  if(psdrv.ConvertTextFile(infile, stream, wrap_lines, cut_lines) == 0)
	    {
	      GXSTD::cerr << "\n";
	      GXSTD::cerr << "Encountered fatal error during conversion" 
			  << "\n";
	      GXSTD::cerr << "Exiting..." << "\n";
	      GXSTD::cerr << "\n";
	      return 1;
	    }
	  psdrv.Epilogue(stream, psdrv.page_count);
	  stream.close();
	  infile.df_Close();
	}
      }
      arg = argv[++narg];
    }
  }
  
  if(num_files == 0) {
    GXSTD::cerr << "\n";
    GXSTD::cerr << "You must enter a file name." << "\n";
    GXSTD::cerr << "Exiting..." << "\n";
    GXSTD::cerr << "\n";
    return 0;
  }

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
