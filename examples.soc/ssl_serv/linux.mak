#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for example program
# C/C++ Compiler Used: gcc version 3.4.4
# --------------------------------------------------------------- 
# Define a name for the executable
PROJECT = server
64BITCFG = 0
FINAL = 1

# GXD/GXS/GXT library path
GCODE_LIB_DIR = ../..

# Setup OpenSSL library path if using downloaded version
#OPENSSL_LIB_DIR = ../../../openssl
#OPENSSL_INC_PATHS = $(OPENSSL_LIB_DIR)$(PATHSEP)include
#OPENSSL_DEFMACS = -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H \
#-DL_ENDIAN -DOPENSSL_BN_ASM_PART_WORDS -DOPENSSL_IA32_SSE2 \
#-DSHA1_ASM -DMD5_ASM -DRMD160_ASM -DAES_ASM 
#OPENSSL_RELEASE_FLAGS = 
#OPENSSL_DEBUG_FLAGS = 
#OPENSSL_LINKER_FLAGS =
#SSL_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)libcrypto.a
#CRYPTO_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)libssl.a

include linux.env

# Setup additional paths for includes and source code
APP_PATH = .

ADD_INC_PATHS = -I../../include -I../../src 

# Compile the files and build the executable
# ===============================================================
all:    server

include project.mak

server:	$(OBJECTS)
	$(CPP) $(COMPILE_FLAGS) $(OBJECTS) $(LINK_LIBRARIES) \
	$(OUTPUT) server $(LINKER_FLAGS)
# ===============================================================

# Remove object files and the executable after running make 
# ===============================================================
clean:
	echo Removing all OBJECT files from working directory...
	rm -f *.o 

	echo Removing EXECUTABLE file from working directory...
	rm -f $(PROJECT)

	echo Removing all test LOG files from working directory...
	rm -f *.log 

	echo Removing all test OUT files from working directory...
	rm -f *.out 

	echo Removing all test EDS files from working directory...
	rm -f *.eds 

	echo Removing all test DATABASE files from working directory...
	rm -f *.gxd 

	echo Removing all test INDEX files from working directory...
	rm -f *.btx 
	rm -f *.gix

	echo Removing all test InfoHog files from working directory...
	rm -f *.ihd 
	rm -f *.ihx 
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
