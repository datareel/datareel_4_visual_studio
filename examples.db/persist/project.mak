$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

grocery$(OBJ_EXT):	grocery.cpp $(GROCERY_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) grocery.cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) grocery$(OBJ_EXT)
