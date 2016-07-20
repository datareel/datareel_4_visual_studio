DataReelCE 4.23 Readme File

CONTENTS
--------
Overview 
License 
Supported 
Platforms Supported 
Compilers 
Package Map 
Example Programs 
Documentation 
Unzipping 
Building The eVC++ 3.0 DLL 
Building The eVC++ 4.0 DLL 

OVERVIEW
--------
What is DataReelCE?
DataReelCE is a DLL for the Window CE family of operating systems. It
is a C++ toolkit used to develop multi-threaded database
communications applications and reusable libraries. DataReelCE is a
derivative of the DataReel toolkit customized for the Window CE family
of operating systems. It was produced by independent work and contract
work released to the public through non-exclusive license
agreements. The initial work began independently in 1997 and was
augmented from 1999 to 2005 by code produced under contract to support
various applications. DataReel toolkit versions 4.0-4.23 were used to
produce the DataReelCE 4.23 toolkit. Some DataReel functionality was
disabled due to Window CE differences. 

Who Can Use DataReelCE?
The DataReelCE toolkit is available to commercial, individual, and
academic developers. 

Who Can Contribute?
The DataReelCE project accepts bug-fixes, enhancements, and additions
from all developers. 

LICENSE
-------
The DataReel open source library is copyrighted by DataReel Software
Development, Copyright (c) 2001-2005, all rights reserved. DataReel
open source is available to non-profit, commercial, individual, and
academic developers. The open-source archive is distributed under the
OSI approved GNU Lesser General Public License (LGPL) with provisions
for use in commercial and non-commercial applications under the terms
of the LGPL. 

Under the terms of the LGPL you can use the open source code in
commercial applications as a static or dynamically linked
library. This allows commercial developers to compile the library and
link to it without making any changes to the DataReel source code. 

GNU Lesser General Public License
This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version. 

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details. 

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA 


SUPPORTED PLATFORMS
-------------------
The following is the current list of operating systems used for
development and cross-platform testing. 

Supported and Original Test Platforms 
* Pocket PC 2000/2002
* HPC Pro 2.11
* Palm Size PC 2.12 (limited support)

Supported Compilers 
The following is the current list of compilers used for development and cross-platform testing. 
C++ Compiler list 
* Microsoft eVC++ 3.0 compiler
* Microsoft eVC++ 4.0 compiler

PACKAGE MAP
-----------
The DataReelCE Windows is not compatible with the DataReel toolkit for
Win32 and UNIX platforms. They should not be unpacked in the same
directory. The directory structure for the distribution is as follows: 

DataReelCE Directory 
* contrib - Empty directory used to install contribution packages
* docs - Directory for all DataReel documentation sets
* eVC++3.0 - Microsoft eVC++ 3.0 DLL and example program project files
* eVC++4.0 - Microsoft eVC++ 4.0 DLL project files
* include - DataReelCE include files
* lib - Empty directory used to install library files
* src - DataReelCE source code files

EXAMPLE PROGRAMS
----------------
The DataReel library includes over 100 console based example
programs. The examples are fully functional programs used to test and
demonstrate each component of the library individually. The example
programs are located within subdirectories of the "examples.gen",
"examples.db", "examples.mt", and "examples.soc" directories. 

Windows CE Makefiles
The DataReelCE library may be compiled either with the appropriate IDE
(eVC++ 3.0 or eVC++ 4.0), or from the MS-DOS command-line using the
batch file (*.bat) for the desired platform. 


DOCUMENTATION
-------------
The DataReel 4.1/4.23 documentation set covers the class library,
example programs, and the utility programs. The class library is
documented by category with a detailed description and usage note for
each function. The DataReel example programs have been converted to an
HTML format with comments/remarks denoted by red lettering. Utility
programs are documented with a brief description and usage
information. 

The complete DataReel 4.1/4.23 documentation set is available in an
HTML format. The DataReel HTML documentation set is distributed with
each DataReel source code distribution. Updated versions and all
changes are posted to the DataReel Web Site: 

http://www.datareel.com/docs.htm 

UNZIPPING
---------
The entire DataReelCE distribution requires approximately 8 MB of free
disk space to unpack. This Windows CE port can only be unzipped under
Windows operating system. It should not be unzipped in the same
directory as the DataReel for either Unix or Windows. 

Windows Installation
To unpack this distribution on a Windows platform you will need a copy
of WINZIP version 6.1 or higher for WIN32. 


BUILDING THE EVC++ 3.0 DLL
--------------------------
The project files for the eVC++ 3.0 compiler are located in the
c:\DataReelCE\eVC++ 3.0\DataReelCE directory. 

DataReelCE.vcn, in the project directory, is the compiler-generated
make file. Each of the supported platforms has a corresponding batch
file which can be used to compile the DataReelCE DLL from the MS-DOS
command-line or by simply clicking the file with Windows Explorer. 

These files are located in the "command-line builds" directory. They
will copy the *.lib and *.dll files to an appropriate directory in
C:\DataReelCE\eVC++ 3.0\LIB 

BUILDING THE EVC++ 4.0 DLL
--------------------------
The project files for the eVC++ 4.0 compiler are located in the
c:\DataReelCE\eVC++ 4.0\DataReelCE directory. 

This directory also contains batch files which allow you to build the
libary for the desired processor from the command line or Windows
Explorer. 

Each batch file will copy the *.lib and *.dll to the an appropriate
directory in C:\DataReelCE\eVC++ 4.0\LIB. 


DataReel Copyright (c) 2001-2005 DataReel Software Development, All
Rights Reserved 

DataReel is a registered trademark (R) of DataReel Software Development 
 
