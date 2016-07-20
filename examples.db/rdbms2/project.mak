PROJECT_DEP = ../rdbms1/gxrdbcon.h

$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

gxrdbcon$(OBJ_EXT):	../rdbms1/gxrdbcon.cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) ../rdbms1/gxrdbcon.cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) gxrdbcon$(OBJ_EXT)
