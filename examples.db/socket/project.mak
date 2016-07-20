$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

dbobject$(OBJ_EXT):	dbobject.cpp $(DBOBJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) dbobject.cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) dbobject$(OBJ_EXT)

