// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: testprog.cpp 
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 10/15/1999 
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

Code used to test the eds configuration manager.

NOTE: Data encoding is a very weak form of asymmetric encryption 
and can be cracked more readily than stronger forms of asymmetric 
encryption. The data encoding routines provided with this library 
should not be implemented in any measure to stop malicious hackers 
on the Internet from breaking into communication systems or from 
compromising encoded data files. The data encoding routines are an 
alternative to sending plain text messages or writing plain text to 
database or configuration files. These routines are simple, fast, 
and integrated into the library so you do not need to include any 
other third party libraries when building code using this library. 
If you need a comprehensive strong encryption toolkit please visit 
the http://www.openssl.org Website to download the latest version 
of OpenSSL.      
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

#include "edscfg.h"

#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Data structure use to store program configuration values
struct ProgramConfig
{
  ProgramConfig() { // Set the default values
    s_val = "YES";
    i_val = 32768;
    l_val = 2147483647;
    d_val = 1.12345;
  }
  ProgramConfig(const ProgramConfig &ob) {
    s_val = ob.s_val; i_val = ob.i_val; l_val = ob.l_val; d_val = ob.d_val;
  }

  UString s_val;
  int i_val;
  long l_val;
  double d_val; 
};

void PausePrg()
{
  cout << "\n";
  cout << "Press <Enter> to continue..." << "\n";
  cin.get();
}

void DumpConfig(edsConfig *edscfg)
{
  edsConfigListNode *ptr = (edsConfigListNode *)edscfg->GetHead();
  while(ptr) {
    cout << ptr->node_data.str << ' ' << "\n";
    ptr = ptr->GetNext();
  }
}

void DisplayConfig(edsConfig *CfgData, ProgramConfig *cfg)
// Display the program configuration and load config values
{
  char *v1 = CfgData->GetStrValue("String");
  if(!v1) {
    cout << "String section missing from config file" << "\n";
  }
  else {
    cout << "String section of config file = " << v1 << "\n";
    cfg->s_val = v1;
  }

  int v2 = CfgData->GetIntValue("Int");
  if(!v2) {
    cout << "Int section missing from config file" << "\n";
  }
  else {
    cout << "Integer section of config file = " << v2 << "\n";
    cfg->i_val = v2;
  }

  double v3 = CfgData->GetFloatValue("Float");
  if(!v3) {
    cout << "Float section missing from config file" << "\n";
  }
  else {
    cout << "Floating Point number section of config file = "
	 << v3 << "\n";
    cfg->d_val = v3;
  }
  
  long v4 = CfgData->GetLongValue("Long");
  if(!v4) {
    cout << "Long int section missing from config file" << "\n";
  }
  else {
    cout << "Long int section of config file = " << v4 << "\n";
    cfg->l_val = v4;
  }

  cout << "\n";
}

void MultipleRead(edsConfig *CfgData)
{
  char *ServerName = CfgData->GetStrValue("ServerName");
  if(!ServerName) {
    cout << "ServerName value is not set in the config file" << "\n";
    return;
  }
  else
    cout << "Server name = " << ServerName << "\n";
  
  char *FullPathName = CfgData->GetStrValue("FullPathName0");
  if(!FullPathName) {
    cout << "No path names are set in the config file" << "\n";
    return;
  }

  char cfgValue[255];
  long file_num = -1;
  
  while(1) {
    file_num++;
    sprintf(cfgValue, "FullPathName%d", (int)file_num);
    char *buf = CfgData->GetStrValue(cfgValue);
    if(!buf) break;

    UString sbuf(buf);
    int offset = 0;
    int index = 0;
    while(1) {
    offset = sbuf.Find("/", offset);
    if(offset != -1) index = offset;
    if(offset == -1) break;
    offset++;
    }
    if(index > 0) sbuf.DeleteAt(0, ++index);

    if(sbuf.length() == 0) {
      cout << "Invalid file name for " << cfgValue << "\n";
      return;
    }
    
    cout << "Reading: " << buf << "\n";
    cout << "Copying to: " << sbuf << "\n";

    buf = 0;
    for(int i = 0; i < 255; i++) cfgValue[i] = 0;
  }
  cout << "\n";
}

void NewParmID(edsConfig *CfgData)
{
  CfgData->UnLoad();
  CfgData->SetParmID("://");
  UString parmName("http");
  CfgData->ReadComments();
  CfgData->ReLoad();
  
  edsConfigListNode *ptr = (edsConfigListNode *)CfgData->GetHead();
  while(ptr) { // Scan until end of list
    int offset = 0;
    offset = ptr->node_data.str.Find(parmName);
    if(offset != -1) {
      UString ServerName(ptr->GetNext()->node_data.str);
      UString FullPathName(ptr->GetNext()->node_data.str);
      offset = ServerName.Find("/");
      if(offset != -1) {
	ServerName.DeleteAt(offset, (ServerName.length() - offset));
	FullPathName.DeleteAt(0, offset);
	cout << "Server: " << ServerName << " Path: " << FullPathName << "\n";
      }
    }
    ptr = ptr->GetNext();
  }
}

void WriteNewConfig(edsConfig *CfgData)
{
  CfgData->WriteConfigLine("Long", "2147483647");
  CfgData->WriteConfigLine("Int", "32768");
  CfgData->WriteConfigLine("Float", "1.12345");
  CfgData->WriteConfigLine("String", "YES");
  CfgData->WriteConfigLine("ServerName", "www.myserver.net");
  CfgData->WriteConfigLine("FullPathName0", "/home1/webpages/index.html");
  CfgData->WriteConfigLine("FullPathName1",
			   "/home1/webpages/images/brick.jpg");
  CfgData->WriteConfigLine("FullPathName2", "/home3/mail_list/list_add.htm");
  CfgData->WriteConfigLine("FullPathName3",
			   "/home3/mail_list/list_remove.htm");
  CfgData->WriteConfigLine("FullPathName4",
			   "/home1/webpages/images/button_bar.jpg");
  CfgData->WriteConfigLine("FullPathName5",
			   "/pub/docs/release_notes/mod1.txt");
  CfgData->WriteConfigLine("FullPathName6",
			   "/pub/docs/release_notes/mod2.txt");
  CfgData->WriteConfigLine("FullPathName7",
			   "/pub/docs/release_notes/mod3.txt");
  CfgData->WriteLine("http://www.myserver.net/home1/webpages/index.html");
  CfgData->WriteLine("http://www.myserver.net/home1/webpages/images/ \
brick.jpg");
  CfgData->WriteLine("http://www.myserver.net/home3/mail_list/list_add.htm");
  CfgData->WriteLine("http://www.myserver.net/home3/mail_list/ \
list_remove.htm");
  CfgData->WriteLine("http://www.myserver.net/home1/webpages/images/ \
button_bar.jpg");
  CfgData->WriteLine("http://www.myserver.net/pub/docs/release_notes/ \
mod1.txt");
  CfgData->WriteLine("http://www.myserver.net/pub/docs/release_notes/ \
mod2.txt");
  CfgData->WriteLine("http://www.myserver.net/pub/docs/release_notes/ \
mod3.txt");
}

int main()
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  edsConfig *CfgData = new edsConfig("testfile.eds");
  ProgramConfig *cfg = new ProgramConfig;
  
  if(!CfgData->Exists()) {
    cout << "\n";
    cout << "The eds config file does not exist!" << "\n";
    cout << "Writing new config file..." << "\n";
    WriteNewConfig(CfgData);
    cout << "\n";
  }

  cout << "Processing the config file" << "\n";
  
  if(CfgData->OpenDatabase()->GetDatabaseError() != gxDBASE_NO_ERROR) {
    cout << CfgData->OpenDatabase()->DatabaseExceptionMessage() << "\n";
    delete CfgData;
    delete cfg;
    return 1;
  }

  CfgData->Load();
  cout << "\n";

  DisplayConfig(CfgData, cfg);
  ProgramConfig org_config(*cfg); // Record the original values
  
  cout << "Testing reload function" << "\n";
  PausePrg();
  CfgData->ReLoad();
  DisplayConfig(CfgData, cfg);

  cout << "Writing new values to the file" << "\n";
  PausePrg();
  if(!CfgData->ChangeConfigValue("String", "NO"))
    cout << "Could not change string value" << "\n";
  if(!CfgData->ChangeConfigValue("Float", 1.98712))
    cout << "Could not change floating point value" << "\n";
  if(!CfgData->ChangeConfigValue("Int", 12345))
    cout << "Could not change integer value" << "\n";
  if(!CfgData->ChangeConfigValue("Long", 23889))
    cout << "Could not change long integer value" << "\n";
  CfgData->ReLoad();
  DisplayConfig(CfgData, cfg);
  
  cout << "Restoring original values" << "\n";
  PausePrg();
  CfgData->ChangeConfigValue("String", org_config.s_val);
  CfgData->ChangeConfigValue("Int", org_config.i_val);
  CfgData->ChangeConfigValue("Float", org_config.d_val);
  CfgData->ChangeConfigValue("Long", org_config.l_val);
  CfgData->ReLoad();
  DisplayConfig(CfgData, cfg);

  cout << "Performing a multiple read and parsing operation" << "\n";
  PausePrg();
  MultipleRead(CfgData);

  cout << "Reloading the file with a new parameter ID string" << "\n";
  PausePrg();
  NewParmID(CfgData);
  
  CfgData->UnLoad();

  delete CfgData;
  delete cfg;
  return 0;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
