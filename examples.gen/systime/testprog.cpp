// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp 
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 12/11/1996
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

This is a test program for the SysTime class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "systime.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

void PausePrg()
{
  cout << "\n";
  cout << "Press <Enter> to continue..." << "\n";
  cin.get();
}

void ClearInputStream(istream &s)
{
  char c;
  s.clear();
  while(s.get(c) && c != '\n') { ; }
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  SysTime *Time = new SysTime;

  cout << "\n";
  cout << "The current system time is: " << Time->GetSystemDateTime() << "\n";
  cout << "The current GMT time is: " << Time->GetGMTDateTime() << "\n"; 
  cout << "\n";
  int days_old = Time->DaysOld(1, 1970);
  cout << days_old << " days have passed since January 1, 1970" << "\n";
  
  PausePrg();
  cout << "Testing the date span function..." << "\n";
  cout << "\n";
  cout << "Enter the number of days to look ahead: ";
  int days; char datespanBuf[255];
  cin >> days;
  if(Time->DateSpan(datespanBuf, days)) 
    cout << "Date span = " << datespanBuf << "\n";
  else
    cout << "DateSpan() error" << "\n";

  ClearInputStream(cin); // Go to end of line
  PausePrg();

  cout << "Testing the leap year function..." << "\n";
  cout << "\n";
  cout << "Enter a year: ";
  int year; 
  cin >> year;
  if(Time->IsLeapYear(year)) 
    cout << year << " is a leap year" << "\n";
  else
    cout << year << " is not a leap year" << "\n";

  cout << "\n";
  cout << "Displaying all the String time formats" << "\n";

  ClearInputStream(cin); // Go to end of line
  PausePrg();

  cout << "AM or PM: " << Time->GetStrTime(SysTime::AMPM) << "\n"; 
  cout << "System date: " << Time->GetStrTime(SysTime::Date) << "\n"; 

  cout << "Day of the month (01..31): " 
       << Time->GetStrTime(SysTime::DayOfTheMonth) << "\n"; 

  cout << "Day of the week (0..6, Sunday=0): " 
       << Time->GetStrTime(SysTime::DayOfTheWeek) << "\n"; 

  cout << "Hour (24 hour clock - 00..23): " 
       << Time->GetStrTime(SysTime::Hour) << "\n";
  
  cout << "Julian day: " << Time->GetStrTime(SysTime::JDay) << "\n"; 
  cout << "Minutes (00..59): " << Time->GetStrTime(SysTime::Minutes) << "\n"; 
  cout << "Month (01..12): " << Time->GetStrTime(SysTime::Month) << "\n"; 
  cout << "Full month name: " << Time->GetStrTime(SysTime::FullMonthName)
       << "\n"; 

  cout << "Abbreviated month name: " 
       << Time->GetStrTime(SysTime::MonthName) << "\n"; 

  cout << "Seconds (00..59): " << Time->GetStrTime(SysTime::Seconds) << "\n"; 
  cout << "System time: " << Time->GetStrTime(SysTime::SystemTime) << "\n"; 
       
  cout << "Timezone name: "
       << Time->GetStrTime(SysTime::TimeZoneName) << "\n";
  
  cout << "Abbreviated weekday name: " 
       << Time->GetStrTime(SysTime::WeekDayName) << "\n";
  
  cout << "Full weekday name: "
       << Time->GetStrTime(SysTime::FullWeekDayName) << "\n";
  
  cout << "Week of the year (00..52, Sunday first): "
       << Time->GetStrTime(SysTime::WeekOfYearSF) << "\n";
  
  cout << "Week of the year (00..52, Monday first): "
       << Time->GetStrTime(SysTime::WeekOfYearMF) << "\n";
  
  cout << "Year (0000..9999): " << Time->GetStrTime(SysTime::Year) << "\n"; 
  cout << "Year (00..99): " << Time->GetStrTime(SysTime::YearXX) << "\n"; 
  
  cout << "\n";

  cout << "Displaying all the Integer time formats" << "\n";
  PausePrg();

  cout << "Day of the month (01..31): " 
       << Time->GetIntTime(SysTime::DayOfTheMonth) << "\n"; 

  cout << "Display the day of the week (0..6, Sunday=0): " 
       << Time->GetIntTime(SysTime::DayOfTheWeek) << "\n"; 

  cout << "Hour (24 hour clock - 00..23): " 
       << Time->GetIntTime(SysTime::Hour) << "\n";
  
  cout << "Julian day: " << Time->GetIntTime(SysTime::JDay) << "\n"; 
  cout << "Minutes (00..59): " << Time->GetIntTime(SysTime::Minutes) << "\n"; 
  cout << "Month (01..12): " << Time->GetIntTime(SysTime::Month) << "\n"; 
  cout << "Seconds (00..59): " << Time->GetIntTime(SysTime::Seconds) << "\n"; 

  cout << "Week of the year (00..52, Sunday first): "
       << Time->GetIntTime(SysTime::WeekOfYearSF) << "\n";
  
  cout << "Week of the year (00..52, Monday first): "
       << Time->GetIntTime(SysTime::WeekOfYearMF) << "\n";
  
  cout << "Year (0000..9999): " << Time->GetIntTime(SysTime::Year) << "\n"; 
  cout << "Year (00..99): " << Time->GetIntTime(SysTime::YearXX) << "\n"; 

  PausePrg();
  
  cout << "Testing invalid format codes..." << "\n";

  PausePrg();

  cout << Time->GetStrTime(9999) << "\n";
  cout << hex << Time->GetIntTime(9999) << "\n";

  cout << "Testing Date/Time formatting functions..." << "\n";

  PausePrg();

  cout << "System time: " << Time->GetSystemTime() << "\n";
  cout << "System date: " << Time->GetSystemDate() << "\n";
  cout << "System time and date: " << Time->GetSystemDateTime() << "\n";
  cout << "GMT time and date: " << Time->GetGMTDateTime() << "\n";
  cout << "SMTP time stamp: " << Time->GetSMTPTime() << "\n";

  delete Time;
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

