# --------------------------------------------------------------- 
# UNIX makefile for the all example programs
# --------------------------------------------------------------- 
all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd cdate; $(MAKE) -f $(MAKEFILE_NAME) all

	cd dcache1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd dcache2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd dfileb; $(MAKE) -f $(MAKEFILE_NAME) all

	cd eds101; $(MAKE) -f $(MAKEFILE_NAME) all

	cd eds201; $(MAKE) -f $(MAKEFILE_NAME) all

	cd edscfg; $(MAKE) -f $(MAKEFILE_NAME) all

	cd fstring; $(MAKE) -f $(MAKEFILE_NAME) all

	cd futils; $(MAKE) -f $(MAKEFILE_NAME) all

	cd logfile; $(MAKE) -f $(MAKEFILE_NAME) all

	cd membuf; $(MAKE) -f $(MAKEFILE_NAME) all

	cd sreg101; $(MAKE) -f $(MAKEFILE_NAME) all

	cd systime; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxconfig; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxlist; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxlistb; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxbstree; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxstring; $(MAKE) -f $(MAKEFILE_NAME) all

	cd terminal; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd cdate; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd dcache1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd dcache2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd dfileb; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd eds101; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd eds201; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd edscfg; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd fstring; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd futils; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd logfile; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd membuf; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd sreg101; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd systime; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxconfig; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxlist; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxlistb; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxbstree; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxstring; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd terminal; $(MAKE) -f $(MAKEFILE_NAME) clean

# Run clean the test programs
# ===============================================================
run:
	cd cdate; ./testprog

	cd dcache1; ./testprog

	cd dcache2; ./testprog testprog.o testfile.o

	cd dfileb; ./testprog

	cd eds101; ./testprog

	cd eds201; ./testprog

	cd edscfg; ./testprog

	cd fstring; ./testprog

	cd futils; ./testprog .

	cd logfile; ./testprog

	cd membuf; ./testprog

	cd sreg101; ./testprog

	cd systime; ./testprog

	cd gxconfig; ./testprog

	cd gxlist; ./testprog

	cd gxlistb; ./testprog

	cd gxbstree; ./testprog

	cd gxstring; ./testprog

	cd terminal; ./testprog
# --------------------------------------------------------------- 
