$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

gxsfile$(OBJ_EXT):	gxsfile.cpp $(GXSFILE_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) gxsfile.cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) gxsfile$(OBJ_EXT)

