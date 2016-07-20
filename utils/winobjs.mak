# --------------------------------------------------------------- 
#  WIN32 Makefile for all the example programs
# --------------------------------------------------------------- 
INSTALL_DIR = ..\..\bin\.

# Compile the object files and build the executables
# ===============================================================
all:
	cd as2htm
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)as2ps
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)codegen
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)edsdump
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)ffind
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)txfilter
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)hexdump
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)db_debug
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)ix_debug
	$(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd as2htm
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)as2ps
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)codegen
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)edsdump
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)ffind
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)txfilter
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)hexdump
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)db_debug
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)ix_debug
	$(MAKE) -f $(MAKEFILE_NAME) clean

# Install the executables
# ===============================================================
install:
	cd as2htm
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)as2ps
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)codegen
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)edsdump
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)ffind
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)txfilter
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)hexdump
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)db_debug
	copy *.exe $(INSTALL_DIR)

	cd ..$(PATH_SEP)ix_debug
	copy *.exe $(INSTALL_DIR)
# --------------------------------------------------------------- 
