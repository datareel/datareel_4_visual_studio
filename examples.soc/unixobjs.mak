# --------------------------------------------------------------- 
# UNIX makefile for the all example programs
# --------------------------------------------------------------- 
all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd arpscan; $(MAKE) -f $(MAKEFILE_NAME) all

	cd base64; $(MAKE) -f $(MAKEFILE_NAME) all

	cd cache; $(MAKE) -f $(MAKEFILE_NAME) all

	cd ftpc; $(MAKE) -f $(MAKEFILE_NAME) all

	cd hostinfo; $(MAKE) -f $(MAKEFILE_NAME) all

	cd httpserv; $(MAKE) -f $(MAKEFILE_NAME) all

	cd ip32; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mac48; $(MAKE) -f $(MAKEFILE_NAME) all

	cd ping; $(MAKE) -f $(MAKEFILE_NAME) all

	cd pingscan; $(MAKE) -f $(MAKEFILE_NAME) all

	cd pop3c; $(MAKE) -f $(MAKEFILE_NAME) all

	cd portscan; $(MAKE) -f $(MAKEFILE_NAME) all

	cd scomserv; $(MAKE) -f $(MAKEFILE_NAME) all

	cd services; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd smtpc; $(MAKE) -f $(MAKEFILE_NAME) all

	cd telnetc; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxdatagm; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxscomm1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxscomm2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxshtml; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxshttpc; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxstream; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxsurl; $(MAKE) -f $(MAKEFILE_NAME) all

	cd bcast; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mcast; $(MAKE) -f $(MAKEFILE_NAME) all

	cd echo; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mthread; $(MAKE) -f $(MAKEFILE_NAME) all

	cd udpscan; $(MAKE) -f $(MAKEFILE_NAME) all

	cd xml; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd arpscan; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd base64; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd cache; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd ftpc; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd hostinfo; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd httpserv; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd ip32; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mac48; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd ping; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd pingscan; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd pop3c; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd portscan; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd scomserv; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd services; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd smtpc; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd telnetc; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxdatagm; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxscomm1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxscomm2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxshtml; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxshttpc; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxstream; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxsurl; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd bcast; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mcast; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd echo; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mthread; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd udpscan; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd xml; $(MAKE) -f $(MAKEFILE_NAME) clean
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
