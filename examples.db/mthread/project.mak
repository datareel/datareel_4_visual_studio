# Build dependency rules
# ===============================================================
PROJECT_DEP = grocery.h m_thread.h

GROCERY_DEP = grocery.h

M_THREAD_DEP = grocery.h m_thread.h m_dbase.h

M_DBASE_DEP = grocery.h m_dbase.h

# ===============================================================

grocery$(OBJ_EXT):	grocery.cpp $(GROCERY_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) grocery.cpp

m_thread$(OBJ_EXT):	m_thread.cpp $(M_THREAD_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) m_thread.cpp

m_dbase$(OBJ_EXT):	m_dbase.cpp $(M_DBASE_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) m_dbase.cpp

$(PROJECT)$(OBJ_EXT):	$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(PROJECT).cpp

OBJECTS = $(PROJECT)$(OBJ_EXT) \
grocery$(OBJ_EXT) m_thread$(OBJ_EXT) m_dbase$(OBJ_EXT)

