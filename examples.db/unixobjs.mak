# --------------------------------------------------------------- 
# UNIX makefile for the all example programs
# --------------------------------------------------------------- 
all:	build_all

build_all:
# Compile the object files and build the executables
# ===============================================================
	cd bigblock; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btnode; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree2; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree3; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree4; $(MAKE) -f $(MAKEFILE_NAME) all

	cd checksum; $(MAKE) -f $(MAKEFILE_NAME) all

	cd crc32; $(MAKE) -f $(MAKEFILE_NAME) all

	cd defrag; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxderror; $(MAKE) -f $(MAKEFILE_NAME) all

	cd filelock; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxfloat; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxint16; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxint32; $(MAKE) -f $(MAKEFILE_NAME) all

	cd persist; $(MAKE) -f $(MAKEFILE_NAME) all

	cd pod; $(MAKE) -f $(MAKEFILE_NAME) all

	cd prevnext; $(MAKE) -f $(MAKEFILE_NAME) all

	cd reclock; $(MAKE) -f $(MAKEFILE_NAME) all

	cd reopen; $(MAKE) -f $(MAKEFILE_NAME) all

	cd simple; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxuint16; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxuint32; $(MAKE) -f $(MAKEFILE_NAME) all

	cd socket; $(MAKE) -f $(MAKEFILE_NAME) all

	cd mthread; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxint64; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxuint64; $(MAKE) -f $(MAKEFILE_NAME) all

	cd gxdbase; $(MAKE) -f $(MAKEFILE_NAME) all

	cd infohog1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree1c; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree2c; $(MAKE) -f $(MAKEFILE_NAME) all

	cd btree3c; $(MAKE) -f $(MAKEFILE_NAME) all

	cd dbcache1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd rdbms1; $(MAKE) -f $(MAKEFILE_NAME) all

	cd rdbms2; $(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd bigblock; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btnode; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree2; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree3; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree4; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd checksum; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd crc32; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd defrag; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxderror; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd filelock; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxfloat; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxint16; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxint32; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd persist; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd pod; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd prevnext; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd reclock; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd reopen; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd simple; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxuint16; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxuint32; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd socket; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd mthread; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxint64; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxuint64; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd gxdbase; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd infohog1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree1c; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree2c; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd btree3c; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd dbcache1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd rdbms1; $(MAKE) -f $(MAKEFILE_NAME) clean

	cd rdbms2; $(MAKE) -f $(MAKEFILE_NAME) clean

# Run all the test programs
# ===============================================================
run:
	cd bigblock; ./testprog

	cd btnode; ./testprog

	cd btree1; ./testprog

	cd btree2; ./testprog

	cd btree3; ./testprog

	cd btree4; ./testprog

	cd checksum; ./testprog

	cd crc32; ./testprog testprog.o

	cd defrag; ./testprog

	cd gxderror; ./testprog

	cd filelock; ./testprog

	cd gxfloat; ./testprog

	cd gxint16; ./testprog

	cd gxint32; ./testprog

	cd persist; ./testprog

	cd pod; ./testprog

	cd prevnext; ./testprog

	cd reclock; ./testprog

	cd reopen; ./testprog

	cd simple; ./testprog

	cd gxuint16; ./testprog

	cd gxuint32; ./testprog

	cd mthread; ./testprog

	cd gxint64; ./testprog

	cd gxuint64; ./testprog

	cd gxdbase; ./testprog

	cd infohog1; ./testprog

	cd btree1c; ./testprog

	cd btree2c; ./testprog

	cd btree3c; ./testprog

	cd dbcache1; ./testprog

	cd rdbms1; ./testprog

	cd rdbms2; ./testprog
# --------------------------------------------------------------- 

