# --------------------------------------------------------------- 
#  WIN32 Makefile for all the example programs
# --------------------------------------------------------------- 

# Compile the object files and build the executables
# ===============================================================
all:
	cd bigblock
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btnode
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree2
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree3
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree4
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)checksum
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)crc32
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)defrag
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxderror
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)filelock
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)persist
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)pod
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)prevnext
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)reclock
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)reopen
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxfloat
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxint16
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxint32
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxuint16
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxuint32
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)socket
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mthread
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxint64
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxuint64
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)gxdbase
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)infohog1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree1c
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree2c
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)btree3c
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)dbcache1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)rdbms1
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)rdbms2
	$(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd bigblock
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btnode
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree2
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree3
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree4
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)checksum
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)crc32
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)defrag
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)filelock
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)persist
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)pod
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)prevnext
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)reclock
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)reopen
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxderror
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxfloat
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxint16
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxint32
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxuint16
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxuint32
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)socket
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mthread
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxint64
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxuint64
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)gxdbase
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)infohog1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree1c
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree2c
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)btree3c
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)dbcache1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)rdbms1
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)rdbms2
	$(MAKE) -f $(MAKEFILE_NAME) clean

# Run all the test programs
# ===============================================================
run:
	cd bigblock
	testprog

	cd ..$(PATH_SEP)btnode
	testprog

	cd ..$(PATH_SEP)btree1
	testprog

	cd ..$(PATH_SEP)btree2
	testprog

	cd ..$(PATH_SEP)btree3
	testprog

	cd ..$(PATH_SEP)btree4
	testprog

	cd ..$(PATH_SEP)checksum
	testprog

	cd ..$(PATH_SEP)crc32
	testprog testprog.obj

	cd ..$(PATH_SEP)defrag
	testprog

	cd ..$(PATH_SEP)filelock
	testprog

	cd ..$(PATH_SEP)persist
	testprog

	cd ..$(PATH_SEP)pod
	testprog

	cd ..$(PATH_SEP)prevnext
	testprog

	cd ..$(PATH_SEP)reclock
	testprog

	cd ..$(PATH_SEP)reopen
	testprog

	cd ..$(PATH_SEP)simple
	testprog

	cd ..$(PATH_SEP)gxderror
	testprog

	cd ..$(PATH_SEP)gxfloat
	testprog

	cd ..$(PATH_SEP)gxint16
	testprog

	cd ..$(PATH_SEP)gxint32
	testprog

	cd ..$(PATH_SEP)gxuint16
	testprog

	cd ..$(PATH_SEP)gxuint32
	testprog

	cd ..$(PATH_SEP)mthread
	testprog

	cd ..$(PATH_SEP)gxint64
	testprog

	cd ..$(PATH_SEP)gxuint64
	testprog

	cd ..$(PATH_SEP)gxdbase
	testprog

	cd ..$(PATH_SEP)infohog1
	testprog

	cd ..$(PATH_SEP)btree1c
	testprog

	cd ..$(PATH_SEP)btree2c
	testprog

	cd ..$(PATH_SEP)btree3c
	testprog

	cd ..$(PATH_SEP)dbcache1
	testprog

	cd ..$(PATH_SEP)rdbms1
	testprog

	cd ..$(PATH_SEP)rdbms2
	testprog
# --------------------------------------------------------------- 



