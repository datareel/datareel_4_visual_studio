#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for example programs
# C/C++ Compiler Used: Microsoft Visual C/C++ 6.0 SP6
# --------------------------------------------------------------- 
# Define a name for the executable
PROJECT = testprog
FINAL = 1
64BITCFG = 0

# Setup OpenSSL library path
# TODO: Set the path to your OPENSSL library install location 
OPENSSL_LIB_DIR = ../../../openssl

# GXD/GXS/GXT library path
GCODE_LIB_DIR = ../..

!include msvc.env

# Common settings for MSVC Open SSL makefiles
OPENSSL_INC_PATHS = $(OPENSSL_LIB_DIR)$(PATHSEP)inc32

OPENSSL_DEFMACS = -DOPENSSL_SYSNAME_WIN32 -DWIN32_LEAN_AND_MEAN \
-DL_ENDIAN -DDSO_WIN32 -D_CRT_SECURE_NO_DEPRECATE -DBN_ASM \
-DMD5_ASM -DSHA1_ASM -DRMD160_ASM /Fdout32 -DOPENSSL_NO_RC5 \
-DOPENSSL_NO_MDC2 -DOPENSSL_NO_KRB5

OPENSSL_RELEASE_FLAGS = /Ox /O2 /Ob2 /WX /Gs0 /GF /Gy /nologo
OPENSSL_DEBUG_FLAGS = /Ox /O2 /Ob2 /WX /Gs0 /GF /Gy /nologo
OPENSSL_LINKER_FLAGS = /nologo /subsystem:console /machine:I386 /opt:ref 

# WIN32 library files
SSL_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)ssleay32$(LIB_EXT)
CRYPTO_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)libeay32$(LIB_EXT)

# Setup additional paths for includes and source code
APP_PATH = .

ADD_INC_PATHS = -I../../include -I../../src 

# Compile the files and build the executable
# ===============================================================
all:	$(PROJECT).exe

!include project.mak

$(PROJECT).exe:	$(OBJECTS) 
	$(LINKER) $(LINKER_FLAGS) \
	$(OBJECTS) $(LINK_LIBRARIES) $(GXLINK_LIB) /OUT:$@ 
# ===============================================================

# Install the binaries to the bin directory
# ===============================================================
install:
	@echo Installing $(PROJECT) binaries to the bin directory...
	@if exist $(PROJECT).exe copy $(PROJECT).exe $(INSTALL_DIR)
# ===============================================================

# Remove OBJS, debug files, and executable after running nmake 
# ===============================================================
clean:
	@echo Removing all .SBR files from working directory...
	if exist *.sbr del *.sbr  

	@echo Removing all .VCW files from working directory...
	if exist *.vcw del *.vcw 

	@echo Removing all .PDB files from working directory...
	if exist *.pdb del *.pdb 

	@echo Removing all .WSP files from working directory...
	if exist *.wsp del *.wsp 

	@echo Removing all .BSC files from working directory...
	if exist *.bsc del *.bsc 

	@echo Removing all .SBT files from working directory...
	if exist *.sbt del *.sbt 

	@echo Removing all .ILK files from working directory...
	if exist *.ilk del *.ilk 

	@echo Removing all .IDB files from working directory...
	if exist *.idb del *.idb 

	@echo Removing all .MDP files from working directory...
	if exist *.mdp del *.mdp 

	@echo Removing all .PCH files from working directory...
	if exist *.pch del *.pch 

	@echo Removing all .NCB files from working directory...
	if exist *.ncb del *.ncb 

	@echo Removing all .RES files from working directory...
	if exist *.res del *.res 

	@echo Removing all $(OBJ_EXT) files from working directory...
	if exist *.obj del *.obj

	@echo Removing the EXECUTABLE file from working directory
	if exist $(PROJECT).exe del $(PROJECT).exe 

	@echo Removing all database files from working directory
	if exist *.ihd del *.idh
	if exist *.ihx del *.idx
	if exist *.hsd del *.hsd
	if exist *.hsx del *.hsx
	if exist *.ds1 del *.ds1
	if exist *.log del *.log
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
