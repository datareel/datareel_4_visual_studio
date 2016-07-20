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

This is a test program for the gxINT16 class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_NATIVE_INT_TYPES__)
int main() { return 0; } // No test for native types
#else

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "gxint16.h"

#if defined (__MSVC__)
#pragma warning(disable:4244) // Function template instantiation warning
#endif

const __SWORD__   SWORDPositiveLimit = 32767;
const __SWORD__   SWORDNegitiveLimit = 32768;
const __USWORD__  USWORDLimit        = 65535;

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
  gxINT16 a = SWORDPositiveLimit;
  cout << "gxINT16 positive limit = " << a << "\n";

  gxINT16 b(SWORDNegitiveLimit);
  cout << "gxINT16 negitive limit = " << b << "\n";

  gxINT16 c;

  c = USWORDLimit;
  cout << "Assigning gxINT16 unsigned limit: " << c << "\n";

  PausePrg();
  
  cout << "Testing gxINT16 copy constructor..." << "\n";
  gxINT16 d(a);
  cout << d << "\n";

  cout << "\n";
  cout << "Testing gxINT16 assignment operator..." << "\n";
  gxINT16 e;
  e = a;
  cout << e << "\n";

  PausePrg();

  cout << "Testing overloaded operators (gxINT16, gxINT16)..." << "\n";
  __SWORD__ buf1, buf2, al, bl;
  
  cout << "Enter first integer: ";
  cin >> buf1;
  if(cin) {
    a = buf1;
    al = buf1;
    cout << "Enter second integer: ";
    cin >> buf2;
  }
  else {
    cout << "Input stream broken. Exiting..." << "\n";
    return 0;
  }
  if(cin) {
    b = buf2;
    bl = buf2;
  }
  else {
    cout << "Input stream broken. Exiting..." << "\n";
    return 0;
  }

  ClearInputStream(cin);
    
  cout << "\n";
  
  OperatorTest(a, b);

  PausePrg();

  cout << "Testing overloaded operators (gxINT16, __LWORD__)..." << "\n";
  OperatorTest(a, bl);

  PausePrg();

  cout << "Testing overloaded operators (__LWORD__, gxINT16)..." << "\n";
  OperatorTest(al, b);
  
  PausePrg();

  cout << "Testing overloaded operators (gxINT16, __WORD__)..." << "\n";
  OperatorTest(a, (__WORD__)bl);

  PausePrg();

  cout << "Testing overloaded operators (__WORD__, gxINT16)..." << "\n";
  OperatorTest((__WORD__)al, b);

  PausePrg();

  cout << "Testing overloaded operators (gxINT16, __SWORD__)..." << "\n";
  OperatorTest(a, (__SWORD__)bl);

  PausePrg();

  cout << "Testing overloaded operators (__SWORD__, gxINT16)..." << "\n";
  OperatorTest((__SWORD__)al, b);

  PausePrg();
  
  cout << "Testing overloaded operators (gxINT16, __UWORD__)..." << "\n";
  OperatorTest(a, (__UWORD__)bl);

  PausePrg();

  cout << "Testing overloaded operators (__UWORD__, gxINT16)..." << "\n";
  OperatorTest((__UWORD__)al, b);

  PausePrg();

  cout << "Testing overloaded operators (gxINT16, __USWORD__)..." << "\n";
  OperatorTest(a, (__USWORD__)bl);

  PausePrg();

  cout << "Testing overloaded operators (__USWORD__, gxINT16)..." << "\n";
  OperatorTest((__USWORD__)al, b);

  PausePrg();
  
  cout << "Testing overloaded operators (gxINT16, __SBYTE__)..." << "\n";
  OperatorTest(a, 'B');

  PausePrg();

  cout << "Testing overloaded operators (__SBYTE__, gxINT16)..." << "\n";
  OperatorTest('A', b);

  return 0;
}

#if defined (__MSVC__)
#pragma warning(default:4244) // Restore previous warning
#endif

#endif // __USE_NATIVE_INT_TYPES__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

