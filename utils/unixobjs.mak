# --------------------------------------------------------------- 
# UNIX makefile for the all example programs
# --------------------------------------------------------------- 
INSTALL_DIR = ../../bin

all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd as2htm; $(MAKE) -f $(MAKEFILE_NAME) all

	cd as2ps; $(MAKE) -f $(MAKEFILE_NAME) all

	cd codegen; $(MAKE) -f $(MAKEFILE_NAME) all

	cd edsdump; $(MAKE) -f $(MAKEFILE_NAME) all

	cd ffind; $(MAKE) -f $(MAKEFILE_NAME) all

	cd txfilter; $(MAKE) -f $(MAKEFILE_NAME) all

	cd hexdump; $(MAKE) -f $(MAKEFILE_NAME) all

	cd db_debug; $(MAKE) -f $(MAKEFILE_NAME) all

	cd ix_debug; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd as2htm; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd as2ps; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd codegen; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd edsdump; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd ffind; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd txfilter; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd hexdump; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd db_debug; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd ix_debug; $(MAKE) -f $(MAKEFILE_NAME) clean

# Install the executables
# ===============================================================
install:
	cd as2htm; cp as2htm $(INSTALL_DIR)/.

	cd as2ps; cp as2ps $(INSTALL_DIR)/.

	cd codegen; cp codegen $(INSTALL_DIR)/.

	cd edsdump; cp edsdump $(INSTALL_DIR)/.

	cd ffind; cp ffind $(INSTALL_DIR)/.

	cd txfilter; cp txfilter $(INSTALL_DIR)/.

	cd hexdump; cp hexdump $(INSTALL_DIR)/.

	cd db_debug; cp db_debug $(INSTALL_DIR)/.

	cd ix_debug; cp ix_debug $(INSTALL_DIR)/.
# --------------------------------------------------------------- 
