# --------------------------------------------------------------- 
#  WIN32 Makefile for all the example programs
# --------------------------------------------------------------- 

# Compile the object files and build the executables
# ===============================================================
all:
	cd cdate
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)dcache1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)dcache2
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)dfileb
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)eds101
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)eds201
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)edscfg
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)fstring
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)futils
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)logfile
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)membuf
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)sreg101
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)systime
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxconfig
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxlist
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxlistb
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxbstree
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxstring
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)terminal
	$(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd cdate
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)dcache1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)dcache2
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)dfileb
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)eds101
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)eds201
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)edscfg
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)fstring
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)futils
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)logfile
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)membuf
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)sreg101
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)systime
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxconfig
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxlist
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxlistb
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxbstree
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxstring
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)terminal
	$(MAKE) -f $(MAKEFILE_NAME) clean

# Run clean the test programs
# ===============================================================
run:
	cd cdate
	testprog

	cd ..$(PATH_SEP)dcache1
	testprog

	cd ..$(PATH_SEP)dcache2
	testprog testprog.obj testfile.obj

	cd ..$(PATH_SEP)dfileb
	testprog

	cd ..$(PATH_SEP)eds101
	testprog

	cd ..$(PATH_SEP)eds201
	testprog

	cd ..$(PATH_SEP)edscfg
	testprog

	cd ..$(PATH_SEP)fstring
	testprog

	cd ..$(PATH_SEP)futils
	testprog .

	cd ..$(PATH_SEP)logfile
	testprog

	cd ..$(PATH_SEP)membuf
	testprog

	cd ..$(PATH_SEP)sreg101
	testprog

	cd ..$(PATH_SEP)systime
	testprog

	cd ..$(PATH_SEP)gxconfig
	testprog

	cd ..$(PATH_SEP)gxlist
	testprog

	cd ..$(PATH_SEP)gxlistb
	testprog

	cd ..$(PATH_SEP)gxbstree
	testprog

	cd ..$(PATH_SEP)gxstring
	testprog

	cd ..$(PATH_SEP)terminal
	testprog
# --------------------------------------------------------------- 
