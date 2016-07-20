// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: r_win32.cpp
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/20/1999
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

Code used by Win32 applications to read and write data to and
from a serial port using overlapped I/O.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

void PrintError(LPCSTR str)
{
  LPVOID lpMessageBuffer;
  int error = GetLastError();
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMessageBuffer,
		0,
		NULL);
  cout << str << ": " << error << " " << lpMessageBuffer << "\n";
  LocalFree(lpMessageBuffer);
}

DWORD CALLBACK ConsoleThread(HANDLE h)
// Starts a user input thread using the console.
// Takes console characters and sends them to the com port.
{
  OVERLAPPED ov;
  HANDLE hconn = GetStdHandle(STD_INPUT_HANDLE);
  BOOL quit = FALSE;
  
  ZeroMemory(&ov, sizeof(ov));
  ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if(ov.hEvent == INVALID_HANDLE_VALUE) {
    PrintError("CreateEvent failed");
    SetCommMask(h, 0);
    return 0;
  }
  
  SetConsoleMode(hconn, 0);
  cout << "Waiting to receive..." << "\n";
  cout << "Press Esc to terminate." << "\n";
  do {
    char buf[10];
    DWORD bytes_read = 0;
    
    // Wait for user data
    WaitForSingleObject(hconn, INFINITE);
    
    // Read the console buffer
    if(!ReadConsole(hconn, buf, sizeof(buf), &bytes_read, NULL)) {
      PrintError("ReadConsole failed");
      quit  = TRUE;
    }
    if(bytes_read) {
      DWORD write;
      // Check for Esc key
      for(write=0; write < bytes_read; write++) {
	// Terminate when Esc key is pressed
	if(buf[write] == 0x1b) {
	  quit = TRUE;
	  break;
	}
      }
      // Send data to the serial device
      if(write) {
	if(!WriteFile(h, buf, write, &write, &ov)) {
	  if(GetLastError() == ERROR_IO_PENDING) {
	    if(!GetOverlappedResult(h, &ov, &write, TRUE)) {
	      PrintError("GetOverlappedResult failed");
	      quit = TRUE;
	    }
	  }
	} else {
	  PrintError("WriteFile failed");
	  quit = TRUE;
	}
      }
    }
    
  } while(!quit);
  
  // Signal the terminal thread to quit
  SetCommMask(h, 0);
  
  return 0;
}

void SerialRead(gxSerialComm *dev, char *dumpfile = 0)
{
  DiskFileB outfile;
  if(dumpfile) {
    cout << "Writing output to the " << dumpfile << " file" << "\n";
    outfile.df_Open(dumpfile);
    if(!outfile) {
      cout << "\n";
      cout << "Cannot open the: " << dumpfile << " capture file"
	   << "\n";
      cout << "Exiting..." << "\n";
      cout << "\n";
      return;
    }
  }

  HANDLE hconn = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE h = dev->DeviceHandle();
  DWORD mask;
  DWORD id;
  OVERLAPPED ov;
    
  HANDLE hConsole = CreateThread(NULL, 0, ConsoleThread, h, 0, &id);
  if(hConsole == INVALID_HANDLE_VALUE) {
    PrintError("CreateThread failed");
    return;
  }
  CloseHandle(hConsole);
  
  ZeroMemory(&ov, sizeof(ov));

  // Create event for overlapped I/O
  ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if(ov.hEvent == INVALID_HANDLE_VALUE)
    PrintError("CreateEvent failed");
  
  // Wait for received characters
  if(!SetCommMask(h, EV_RXCHAR))
    PrintError("SetCommMask failed");
  
  while(1) {
    // Get the event mask
    if(!WaitCommEvent(h, &mask, &ov)) {
      DWORD e = GetLastError();
      if(e == ERROR_IO_PENDING) {
	DWORD r;
	if(!GetOverlappedResult(h, &ov, &r, TRUE)) {
	  PrintError("GetOverlappedResult failed");
	  break;
	}
      } else {
	PrintError("WaitCommEvent failed");
	break;
      }
    }
    
    // If no event, then UI thread terminated with SetCommMask(h, 0)
    if(mask == 0) {
      break;
    }

    if(mask & EV_RXCHAR) {
      char buf[10];
      DWORD bytes_read;
      do {
	bytes_read = 0;
	if(!ReadFile(h, buf, sizeof(buf), &bytes_read, &ov)) {
	  if(GetLastError() == ERROR_IO_PENDING) {
	    if(!GetOverlappedResult(h, &ov, &bytes_read, TRUE)) {
	      PrintError("GetOverlappedResult failed");
	      break;
	    }
	  } else {
	    PrintError("ReadFile failed");
	    break;
	  }
	}
	if(bytes_read) {
	  if(dumpfile) { // Capture the data to a file
	    outfile.df_Write((char *)buf, bytes_read);
	    outfile.df_Flush();
	  }
	  else // Write the data to the console
	    WriteFile(hconn, buf, bytes_read, &bytes_read, NULL);
	}
      } while(bytes_read);
    }
    mask = 0;
  }
  outfile.df_Close();
  CloseHandle(ov.hEvent);  // Close the event
}
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
