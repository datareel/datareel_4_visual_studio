# --------------------------------------------------------------- 
#  WIN32 Makefile for all the example programs
# --------------------------------------------------------------- 

# Compile the object files and build the executables
# ===============================================================
all:
	cd arpscan
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)base64
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)cache
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)ftpc
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)hostinfo
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)httpserv
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)ip32
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mac48
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)ping
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)pingscan
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)pop3c
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)portscan
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)scomserv
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)services
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple2
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple3
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)smtpc
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)telnetc
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxdatagm
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxscomm1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxscomm2
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxshtml
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxshttpc
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxstream
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxsurl
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)winsock
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)bcast
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)echo
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mthread
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mcast
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)udpscan
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)xml
	$(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd arpscan
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)base64
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)cache
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)ftpc
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)hostinfo
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)httpserv
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)ip32
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mac48
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)ping
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)pingscan
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)pop3c
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)portscan
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)scomserv
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)services
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple2
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple3
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)smtpc
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)telnetc
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxdatagm
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxscomm1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxscomm2
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxshtml
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxshttpc
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxstream
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxsurl
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)winsock
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)bcast
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mcast
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)echo
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mthread
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)udpscan
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)xml
	$(MAKE) -f $(MAKEFILE_NAME) clean

# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
