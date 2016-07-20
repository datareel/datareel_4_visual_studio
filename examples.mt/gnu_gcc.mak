#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for the all example programs
# --------------------------------------------------------------- 

# Define a macros
MAKE = make
MAKEFILE_NAME = gnu_gcc.mak

all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd cancel; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cond1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cond2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cond3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd destroy; $(MAKE) -f $(MAKEFILE_NAME) all

	cd detach; $(MAKE) -f $(MAKEFILE_NAME) all

	cd exiting; $(MAKE) -f $(MAKEFILE_NAME) all

	cd joined; $(MAKE) -f $(MAKEFILE_NAME) all

	cd message; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mutex1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mutex2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd prio1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd prio2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd sema1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd sema2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd sema3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd sizeof; $(MAKE) -f $(MAKEFILE_NAME) all

	cd specific; $(MAKE) -f $(MAKEFILE_NAME) all

	cd stack; $(MAKE) -f $(MAKEFILE_NAME) all

	cd suspend; $(MAKE) -f $(MAKEFILE_NAME) all

	cd timed; $(MAKE) -f $(MAKEFILE_NAME) all

	cd tpool1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd tpool2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd httpserv; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxdbase; $(MAKE) -f $(MAKEFILE_NAME) all

	cd template; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd cancel; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cond1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cond2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cond3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd destroy; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd detach; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd exiting; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd joined; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd message; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mutex1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mutex2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd prio1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd prio2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd sema1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd sema2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd sema3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd sizeof; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd specific; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd stack; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd suspend; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd timed; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd tpool1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd tpool2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd httpserv; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxdbase; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd template; $(MAKE) -f $(MAKEFILE_NAME) clean
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################


