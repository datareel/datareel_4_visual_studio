#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for the all example programs
# --------------------------------------------------------------- 
all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd cancel; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cond1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cond2; $(MAKE) -f $(MAKEFILE_NAME) all

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

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd specific; $(MAKE) -f $(MAKEFILE_NAME) all

	cd stack; $(MAKE) -f $(MAKEFILE_NAME) all

	cd suspend; $(MAKE) -f $(MAKEFILE_NAME) all

	cd timed; $(MAKE) -f $(MAKEFILE_NAME) all

	cd tpool1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd tpool2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd template; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd cancel; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cond1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cond2; $(MAKE) -f $(MAKEFILE_NAME) clean

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

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd specific; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd stack; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd suspend; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd timed; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd tpool1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd tpool2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd template; $(MAKE) -f $(MAKEFILE_NAME) clean

# Run all the test programs
# ===============================================================
run:
	cd cancel; ./testprog

	cd cond1; ./testprog

	cd cond2; ./testprog

	cd destroy; ./testprog

	cd detach; ./testprog

	cd exiting; ./testprog

	cd joined; ./testprog

	cd message; ./testprog

	cd mutex1; ./testprog

	cd mutex2; ./testprog

	cd prio1; ./testprog

	cd prio2; ./testprog

	cd sema1; ./testprog

	cd sema2; ./testprog

	cd simple1; ./testprog

	cd simple2; ./testprog

	cd simple3; ./testprog

	cd specific; ./testprog

	cd stack; ./testprog

	cd suspend; ./testprog

	cd timed; ./testprog

	cd tpool1; ./testprog

	cd tpool2; ./testprog

	cd template; ./testprog
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################


