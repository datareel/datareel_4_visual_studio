# Compile the object files and build the executables
# ===============================================================
all:
	cd cancel
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)cond1         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)cond2         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)destroy       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)detach        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)exiting       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)joined        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)message       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mutex1        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)mutex2        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)prio1         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)prio2         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)sema1         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)sema2         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple1       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple2       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)simple3       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)specific      
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)stack         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)suspend       
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)timed         
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)tpool1        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)tpool2        
	$(MAKE) -f $(MAKEFILE_NAME) all

	cd ..$(PATH_SEP)template  
	$(MAKE) -f $(MAKEFILE_NAME) all

# Remove OBJS, debug files, and executables after running nmake 
# ===============================================================
clean:
	cd cancel
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)cond1         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)cond2         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)destroy       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)detach        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)exiting       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)joined        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)message       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mutex1        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)mutex2        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)prio1         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)prio2         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)sema1         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)sema2         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple1       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple2       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)simple3       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)specific      
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)stack         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)suspend       
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)timed         
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)tpool1        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)tpool2        
	$(MAKE) -f $(MAKEFILE_NAME) clean

	cd ..$(PATH_SEP)template  
	$(MAKE) -f $(MAKEFILE_NAME) clean


# Run clean the test programs
# ===============================================================
run:
	cd cancel
	testprog

	cd ..$(PATH_SEP)cond1         
	testprog

	cd ..$(PATH_SEP)cond2         
	testprog

	cd ..$(PATH_SEP)destroy       
	testprog

	cd ..$(PATH_SEP)detach        
	testprog

	cd ..$(PATH_SEP)exiting       
	testprog

	cd ..$(PATH_SEP)joined        
	testprog

	cd ..$(PATH_SEP)message       
	testprog

	cd ..$(PATH_SEP)mutex1        
	testprog

	cd ..$(PATH_SEP)mutex2        
	testprog

	cd ..$(PATH_SEP)prio1         
	testprog

	cd ..$(PATH_SEP)prio2         
	testprog

	cd ..$(PATH_SEP)sema1         
	testprog

	cd ..$(PATH_SEP)sema2         
	testprog

	cd ..$(PATH_SEP)simple1       
	testprog

	cd ..$(PATH_SEP)simple2       
	testprog

	cd ..$(PATH_SEP)simple3       
	testprog

	cd ..$(PATH_SEP)specific      
	testprog

	cd ..$(PATH_SEP)stack         
	testprog

	cd ..$(PATH_SEP)suspend       
	testprog

	cd ..$(PATH_SEP)timed         
	testprog

	cd ..$(PATH_SEP)tpool1        
	testprog

	cd ..$(PATH_SEP)tpool2        
	testprog

	cd ..$(PATH_SEP)template  
	testprog
# ===============================================================

