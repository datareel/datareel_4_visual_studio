// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 03/21/1997 
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

This is a test program for the terminal interface class.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

// This directive must be defined to use the terminal I/O class
#if !defined (__USE_TERMINAL_IO__)
int main() { return 0; } // No test for native types
#else

#include <ctype.h>
#include <string.h>
#include "terminal.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

const int selections = 10;
const int functions = 10;
typedef void (*MF)(); // Function pointer to menu functions

void Exit();
void CursorPositionTest(); // Full screen cursor test
void Dialogs();            // User interface functions
void InputFunctions();     // Test input functions
void StatusLine();         // Status line function
void Justification();      // Test right, left, and center justification
void VideoTest();          // Test video attributes 
void KBWait();             // Test KBWait function
void ScanCodes();          // Display keyboard scan codes
void GetTerm();            // Get the terminal type

const char *MainMenu[selections] = {
  "(0) - To exit this program",
  "(1) - Cursor position test",
  "(2) - Standard dialog test",
  "(3) - Input function test",
  "(4) - Status line test",
  "(5) - Line justification test",
  "(6) - Video attributes test",
  "(7) - Key board wait test",
  "(8) - Display keyboard scancodes",
  "(9) - Get the terminal type"
};

MF MMFunctions[functions] = {
  &Exit,
  &CursorPositionTest, // Full screen cursor test
  &Dialogs,            // User interface functions
  &InputFunctions,     // Test input functions
  &StatusLine,         // Status line function
  &Justification,      // Test right, left, and center justification
  &VideoTest,          // Test video attributes 
  &KBWait,             // Test KBWait function
  &ScanCodes,          // Display keyboard scan codes
  &GetTerm             // Get the terminal type
};

void Menu()
{
  const char *title = "TERMINAL CLASS TEST PROGRAM'S MAIN MENU" ;

  Coords *p = new Coords(0, 0);
  p->SetX(terminal->Center(title));
  p->SetY(terminal->ScreenCenter(selections));
  terminal->ClearScreen();
  terminal->Write(title, p->XPos(), p->YPrev());

  for(int i = 0; i < selections; i++)
    terminal->Write(MainMenu[i], p->XPos(), p->YOffset(1));
  
  terminal->Write((char *)"Press the number of your selection >",
		  p->XPos(), p->YOffset(2));
  delete p;
}

void ProcessArgs()
{
  int c;
  while(1) {
    Menu();
    c = terminal->GetChar();
    switch(tolower(c)) {
      case '0': 
	(*MMFunctions[0]) (); // Call the appropriate menu function
	return;
      case '1':
	(*MMFunctions[1]) (); // Call the appropriate menu function
	break;
      case '2':
	(*MMFunctions[2]) (); // Call the appropriate menu function
	break;
      case '3':
	(*MMFunctions[3]) (); // Call the appropriate menu function
	break;
      case '4':
	(*MMFunctions[4]) (); // Call the appropriate menu function
	break;
      case '5':
	(*MMFunctions[5]) (); // Call the appropriate menu function
	break;
      case '6':
	(*MMFunctions[6]) (); // Call the appropriate menu function
	break;
      case '7':
	(*MMFunctions[7]) (); // Call the appropriate menu function
	break;
      case '8':
	(*MMFunctions[8]) (); // Call the appropriate menu function
	break;
      case '9':
	(*MMFunctions[9]) (); // Call the appropriate menu function
	break;
      default:
	break;
    }
  }
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  terminal->init();   // Initialize the terminal

  ProcessArgs();      // Enter main communications loop

  terminal->ClearScreen(); 
  terminal->finish(); // Restore the terminal before exiting
  return 0;
}

void ScanCodes()
{
  char c; int x, y;
  terminal->ClearScreen();
  x = y = 0;
  terminal->Write((char *)"Displaying keyboard scan codes...", x, y);
  y = y+2;
  terminal->Write((char *)"Type keys. Press Esc when finished",x ,y);
  y = y+2;
  terminal->MoveCursor(x, y);
  while ((c = terminal->GetChar()) != termEscapeKey) {
    y = y+1;
    terminal->Write((char *)"The ", x, y);
    terminal->Write(c);
    terminal->Write((char *)" key value is: ");
    terminal->Write((int)c & 255);
    if(y > terminal->MaxLines()-1) {
      terminal->ClearScreen();
      terminal->Write((char *)"Type keys. Press Esc when finished",0 ,0);
      y = 2;
      terminal->MoveCursor(x, y);
    }
  }
  terminal->ClearScreen();
}

void CursorPositionTest()
{
  terminal->ClearScreen();
  
  const char Pop = '*';     // Display cursor position character
  short x = 0, y = 0;
  unsigned char lastc = 0;
  unsigned char c = 0;
  char *sl = "U-up, D-down, L-left, R-right, Esc to quit...";
  int len = strlen(sl);
  
  while (c != termEscapeKey) {
    terminal->StatusLine(sl);
    terminal->Write((char *)"x: ", len + 5, terminal->MaxLines()-1);
    terminal->Write(x);
    terminal->Write((char *)" y: ");
    terminal->Write(y);
    terminal->MoveCursor(x, y);
    c = terminal->GetChar();
    switch (tolower(c))	{
      case ' ': case '\r':
	terminal->Write(Pop, x, y);
	terminal->PutBack(lastc);
	break;
      case 'u': // up cursor
	if (y-- == 0) y = terminal->MaxLines()-1;
	lastc = c;
	break;
      case 'd': // down cursor
	if (++y == terminal->MaxLines()-1) y = 0; // Avoid  status line
	lastc = c;
	break;
      case 'l': // left cursor
	if (x-- == 0) x = terminal->MaxCols()-1;
	lastc = c;
	break;
      case 'r': //  right cursor
	if (++x == terminal->MaxCols()) x = 0;
	lastc = c;
	break;
      default:
	break;
    }
  }
  terminal->ClearScreen();
}

void VideoTest()
{
  int x, y;
  x = y = 0;
  terminal->ClearScreen();
  terminal->Write((char *)"Testing video attributes...", x, y);
  y = y+2;
  terminal->StandOut((char *)"Stand Out video mode", x, y);
  y = y+2;
  terminal->Write((char *)"Normal text", x, y);
  y = y+2;
  terminal->AnyKey(x, y);
  terminal->ClearScreen();
}

void Justification()
{
  terminal->ClearScreen();
  terminal->Write((char *)"Testing left, center, and right justification", 0, 0);
  char *fox = "This quick brown fox jumps over the lazy dog";
  char *nums = "0123456789";
  char *alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  terminal->Write(nums, 0, 2);
  terminal->Write(nums, terminal->Center(nums), 4);
  terminal->Write(nums, terminal->Right(nums), 6);

  terminal->Write(alpha, 0, 8);
  terminal->Write(alpha, terminal->Center(alpha), 10);
  terminal->Write(alpha, terminal->Right(alpha), 12);

  terminal->Write(fox, 0, 14);
  terminal->Write(fox, terminal->Center(fox), 16);
  terminal->Write(fox, terminal->Right(fox), 18);

  terminal->AnyKey(0, 20);
  terminal->ClearScreen();
}

void InputFunctions()
{
  terminal->ClearScreen();
  Coords *pos = new Coords(0, 0);
  terminal->Write((char *)"Testing input functions...", pos->XPos(), pos->YPos());

  terminal->Write((char *)"Enter a string: ", pos->XPos(), pos->YOffset(2)); 
  char buf[81];
  terminal->GetString(buf);
  terminal->Write((char *)"You entered: ", pos->XPos(), pos->YOffset(2)); 
  terminal->Write(buf);

  terminal->Write((char *)"Enter a password: ", pos->XPos(), pos->YOffset(2)); 
  terminal->GetPassword(buf);
  terminal->Write((char *)"You entered: ", pos->XPos(), pos->YOffset(2)); 
  terminal->Write(buf);

  terminal->Write((char *)"Enter a number: ", pos->XPos(), pos->YOffset(2)); 
  int num = terminal->GetInt();
  terminal->Write((char *)"You entered: ", pos->XPos(), pos->YOffset(2)); 
  terminal->Write(num); 

  terminal->Write((char *)"Enter a floating point number: ",
		  pos->XPos(), pos->YOffset(2)); 
  double fp = terminal->GetFloat();
  terminal->Write((char *)"You entered: ", pos->XPos(), pos->YOffset(2)); 
  terminal->Write(fp);

  terminal->AnyKey(pos->XPos(), pos->YOffset(2)); 
  terminal->ClearScreen();
  delete pos;
}

void KBWait()
{
  terminal->ClearScreen();
  terminal->Write((char *)"Testing KBWait() function...", 0, 0);
  terminal->AnyKey(0, 1);
  
  int i, j = 0; 
  for(;;) { // Continue processing until any key is pressed
    for(i = 0; i < terminal->MaxCols(); i++) {
      terminal->Write('*', i, j);
    }
    j++;
    if(terminal->KBWait()) break; 
  }
  terminal->ClearScreen();
}

void Dialogs()
{
  Coords *pos = new Coords(0, 0);
  terminal->ClearScreen();
  terminal->Write((char *)"Testing standard dialogs functions...",
		  pos->XPos(), pos->YPos()); 

  int yn = terminal->YesNo(pos->XPos(), pos->YOffset(2)); 
  if(yn) 
    terminal->Write((char *)"You pressed \'y\' to continue",
		    pos->XPos(), pos->YOffset(2)); 

  terminal->MoveCursor(pos->XPos(), pos->YOffset(2)); 
  yn = terminal->YesNo();
  if(!yn) 
    terminal->Write((char *)"You pressed \'n\' not to continue",
		    pos->XPos(), pos->YOffset(2)); 

  terminal->AnyKey(pos->XPos(), pos->YOffset(2)); 

  terminal->ClearScreen();
  delete pos;
}

void StatusLine()
{
  terminal->ClearScreen();
  terminal->Write((char *)"Testing status line functions...",0, 0);
  terminal->StatusLine((char *)"Normal mode bottom, left justified status line");
  terminal->AnyKey(0, 2);
  terminal->ClearScreen();
}

void GetTerm()
{
  terminal->ClearScreen();
  if(getenv("TERM")) {
    terminal->Write((char *)"The TERM variable is set: ", 0, 0); 
    terminal->Write(terminal->GetTerm());
  }
  else
    terminal->Write((char *)"The TERM variable is not set.", 0, 0); 

  terminal->Write((char *)"The maximum number of lines = ", 0, 1);
  terminal->Write(terminal->MaxLines());
  terminal->Write((char *)"The maximum number of columns = ", 0, 2);
  terminal->Write(terminal->MaxCols());
  terminal->AnyKey(0, 4);
  terminal->ClearScreen();
}

void Exit()
{
  return; 
}

#endif // __USE_TERMINAL_IO__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
