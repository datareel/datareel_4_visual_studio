// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 02/14/1996  
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

This is a test program for the database error functions and
exception classes.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include <stdlib.h>
#include "gxderror.h"

#ifdef __CPP_EXCEPTIONS__

class CTest
{
public:
  CTest() { cout << "Constructing CTest object..." << "\n"; }
  ~CTest() { cout << "Destructing CTest object..." << "\n"; }

public:
  int FOpen(const char *s);
  gxDatabaseError GetError() { return err; }
  
private:
  gxDatabaseError err;
  // ...
  // ...
  // Other data members
};

int CTest::FOpen(const char *s)
{
  // ...
  // ...
  // Simulate an fopen error
  // Error detected here
  cout << "In CTest::FOpen(). Throwing a gxCDatabaseException exception..."
       << "\n";
  err = gxDBASE_FILE_OPEN_ERROR;
  throw gxCDatabaseException();
  // This will automatically call the destructor function during
  // stack unwinding for all local objects constructed before
  // the exception was thrown.
  
  // The context between the throw site and the catch handler
  // is known as the "exception stack frame". 
  // ...
  // ...

  return 0;   
}
#endif

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

int main()
{
  cout << "\n";
  cout << "Displaying an error message..." << "\n";

  gxDatabaseError err = gxDBASE_WRONG_FILE_TYPE;
  
  cout << gxDatabaseExceptionMessage(err) << "\n";
  
  PausePrg();
  
#ifdef __CPP_EXCEPTIONS__
  cout << "Testing C++'s built-in exception handling routines" << "\n";

  CTest ob;
  try
    {
      cout << "In try block, calling CTest::FOpen()..." << "\n";
      ob.FOpen("ABCDEF");
    }

  catch(gxCDatabaseException)
    {
      cout << gxDatabaseExceptionMessage(ob.GetError()) << "\n";
    }

  cout << "Resuming program execution here..." << "\n";
#else
  cout << "C++ Exception handling is not enabled" << "\n";
#endif

  PausePrg();
  cout << "Test complete. Exiting..." << "\n";

  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
