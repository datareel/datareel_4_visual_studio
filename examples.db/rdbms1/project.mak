PROJECT_DEP = gxrdbcon.h

$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

gxrdbcon$(OBJ_EXT):	gxrdbcon.cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) gxrdbcon.cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) gxrdbcon$(OBJ_EXT)
