// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/05/1997  
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

This is a test program for the FLOAT64 class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_NATIVE_FLOAT_TYPES__)
int main() { return 0; } // No test for native types
#else

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxfloat.h"
#include "dfileb.h"

#if defined (__MSVC__)
#pragma warning(disable:4244) // Function template instantiation warning
#endif

const __DPFLOAT__ DPFLOATPositiveLimit = 1.7E+308; // (15 digit precision)
const __DPFLOAT__ DPFLOATNegitiveLimit = 1.7E-308; // (15 digit precision)

void PausePrg()
{
  cout << "\n";
  cout << "Press enter to continue..." << "\n";
  cin.get();
}

void ClearInputStream(istream &s)
// Used to clear istream
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

void FWrite(char *fname)
{
  gxFLOAT64 aa = 3.141592653589793; // Arc cosine of -1 equals pi
  cout.precision(15);  // Show 15 places after the decimal point 
  cout.setf(ios::fixed | ios::right); // Normal FP notation right justified
  cout << "Writing a gxFLOAT64 value " << aa << " to a file..." << "\n";

  DiskFileB stream(fname, DiskFileB::df_READWRITE, DiskFileB::df_CREATE);
  if(!stream) {
    cout << "Could not open " << fname << " file" << "\n";
    return;
  }
  stream.df_Write(&aa, sizeof(gxFLOAT64));
  cout << "Wrote " << stream.df_pcount() << " bytes" << "\n";
  stream.df_Close();
}

void FRead(char *fname)
{
  gxFLOAT64 bb = 0;
  cout.precision(15);  // Show 15 places after the decimal point 
  cout.setf(ios::fixed | ios::right); // Normal FP notation right justified
  cout << "Reading back gxFLOAT64 value from the file..." << "\n";

  DiskFileB stream(fname);
  if(!stream) {
    cout << "Could not open " << fname << " file" << "\n";
    return;
  }
  stream.df_Read(&bb, sizeof(gxFLOAT64));
  cout << "Read " << stream.df_gcount() << " bytes" << "\n";
  stream.df_Close();
  cout << "Value read from file = " << bb << "\n";
}

template<class TYPEX, class TYPEY>
inline void OperatorTest(TYPEX x, TYPEY y)
{
  char *tf[2] = {"FALSE", "TRUE"};
  cout << "\n";
  cout << "Value x = " << x << ", Value y = " << y << "\n";
  TYPEX z;
  cout << x << " += " << y << " = "; z = x; z += y; cout << z << "\n";
  cout << x << " -= " << y << " = "; z = x; z -= y; cout << z << "\n";
  cout << x << " *= " << y << " = "; z = x; z *= y; cout << z << "\n";
  cout << x << " /= " << y << " = "; z = x; z /= y; cout << z << "\n";
  cout << x << " * " << y << " = " ; z = (x * y); cout << z << "\n";
  cout << x << " / " << y << " = " ; z = (x / y); cout << z << "\n";
  cout << x << " + " << y << " = " ; z = (x + y); cout << z << "\n";
  cout << x << " - " << y << " = " ; z = (x - y); cout << z << "\n";
  cout << x << " == " << y << " = " << tf[(x == y)] << "\n";
  cout << x << " != " << y << " = " << tf[(x != y)] << "\n";
  cout << x << " <= " << y << " = " << tf[(x <= y)] << "\n";
  cout << x << " >= " << y << " = " << tf[(x >= y)] << "\n";
  cout << x << " < " << y << " = " << tf[(x < y)] << "\n";
  cout << x << " > " << y << " = " << tf[(x > y)] << "\n";
  cout << x << "++ = "; z = x; z++; cout << z << "\n";
  cout << x << "-- = "; z = x; z--; cout << z << "\n"; 
  cout << "++" << x << " = "; z = x; ++z; cout << z << "\n";
  cout << "--" << x << " = "; z = x; --z; cout << z << "\n";
}

int main()
{
  gxFLOAT64 a = DPFLOATPositiveLimit;
  cout << "gxFLOAT64 positive limit = " << a << "\n";

  gxFLOAT64 b(DPFLOATNegitiveLimit);
  cout << "gxFLOAT64 negitive limit = " << b << "\n";

  PausePrg();
  
  cout << "Testing gxFLOAT64 copy constructor..." << "\n";
  gxFLOAT64 c(a);
  cout << c << "\n";

  cout << "\n";
  cout << "Testing gxFLOAT64 assignment operator..." << "\n";
  gxFLOAT64 d;
  d = a;
  cout << d << "\n";

  PausePrg();

  // Reading and writing a gxFLOAT64 value to a file
  char fname[255] = "testf64.out";
  FILE *tmp; // Temporary file pointer
  tmp = ::fopen(fname, "rb");

  if(tmp) FRead(fname); else FWrite(fname);
    
  PausePrg();
  
  cout.precision(10);  // Show 10 places after the decimal point 
  cout.setf(ios::fixed | ios::right); // Normal FP notation right justified
  cout << "Testing overloaded operators (gxFLOAT64, gxFLOAT64)..." << "\n";
  __DPFLOAT__ buf1, buf2, af, bf;
  
  cout << "Enter first floating point value: ";
  cin >> buf1;
  if(cin) {
    a = buf1;
    af = buf1;
    cout << "Enter second floating point value: ";
    cin >> buf2;
  }
  else {
    cout << "Input stream broken. Exiting..." << "\n";
    return 0;
  }
  if(cin) {
    b = buf2;
    bf = buf2;
  }
  else {
    cout << "Input stream broken. Exiting..." << "\n";
    return 0;
  }

  ClearInputStream(cin);
    
  cout << "\n";
  
  OperatorTest(a, b);
  
  PausePrg();

  cout << "Testing overloaded operators (gxFLOAT64, __DPFLOAT__)..." << "\n";
  OperatorTest(a, bf);

  PausePrg();

  cout << "Testing overloaded operators (__DPFLOAT__, gxFLOAT64)..." << "\n";
  OperatorTest(af, b);
  
  PausePrg();

  cout << "Testing overloaded operators (gxFLOAT64, __LWORD__)..." << "\n";
  OperatorTest(a, (__LWORD__)bf);

  PausePrg();

  cout << "Testing overloaded operators (__LWORD__, gxFLOAT64)..." << "\n";
  OperatorTest((__LWORD__)af, b);
  
  PausePrg();

  cout << "Testing overloaded operators (gxFLOAT64, __WORD__)..." << "\n";
  OperatorTest(a, (__WORD__)bf);

  PausePrg();

  cout << "Testing overloaded operators (__WORD__, gxFLOAT64)..." << "\n";
  OperatorTest((__WORD__)af, b);

  PausePrg();

  cout << "Testing overloaded operators (gxFLOAT64, __SWORD__)..." << "\n";
  OperatorTest(a, (__SWORD__)bf);

  PausePrg();

  cout << "Testing overloaded operators (__SWORD__, gxFLOAT64)..." << "\n";
  OperatorTest((__SWORD__)af, b);

  PausePrg();
  
  cout << "Testing overloaded operators (gxFLOAT64, __UWORD__)..." << "\n";
  OperatorTest(a, (__UWORD__)bf);

  PausePrg();

  cout << "Testing overloaded operators (__UWORD__, gxFLOAT64)..." << "\n";
  OperatorTest((__UWORD__)af, b);

  PausePrg();

  cout << "Testing overloaded operators (gxFLOAT64, __USWORD__)..." << "\n";
  OperatorTest(a, (__USWORD__)bf);

  PausePrg();

  cout << "Testing overloaded operators (__USWORD__, gxFLOAT64)..." << "\n";
  OperatorTest((__USWORD__)af, b);

  PausePrg();
  
  cout << "Testing overloaded operators (gxFLOAT64, __SBYTE__)..." << "\n";
  OperatorTest(a, 'B');

  PausePrg();

  cout << "Testing overloaded operators (__SBYTE__, gxFLOAT64)..." << "\n";
  OperatorTest('A', b);

  return 0;
}

#if defined (__MSVC__)
#pragma warning(default:4244) // Restore previous warning
#endif

#endif // __USE_NATIVE_FLOAT_TYPES__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
