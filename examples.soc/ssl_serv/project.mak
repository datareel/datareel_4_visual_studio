# Build dependency rules
# ===============================================================
include ../../env/glibdeps.mak

PROJECT_DEP = $(APP_PATH)$(PATHSEP)m_thread.h

M_THREAD_DEP = $(APP_PATH)$(PATHSEP)m_thread.h
# ===============================================================

include ../../env/glibobjs.mak

m_thread$(OBJ_EXT):	$(APP_PATH)$(PATHSEP)m_thread.cpp $(M_THREAD_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(APP_PATH)$(PATHSEP)m_thread.cpp

$(PROJECT)$(OBJ_EXT):	$(APP_PATH)$(PATHSEP)$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(APP_PATH)$(PATHSEP)$(PROJECT).cpp

# Object files
OBJECTS = $(PROJECT)$(OBJ_EXT) m_thread$(OBJ_EXT) gxssl$(OBJ_EXT) \
asprint$(OBJ_EXT) \
bstreei$(OBJ_EXT) \
btcache$(OBJ_EXT) \
btnode$(OBJ_EXT) \
btstack$(OBJ_EXT) \
cdate$(OBJ_EXT) \
dbasekey$(OBJ_EXT) \
dbfcache$(OBJ_EXT) \
dbugmgr$(OBJ_EXT) \
devcache$(OBJ_EXT) \
dfileb$(OBJ_EXT) \
eds101$(OBJ_EXT) \
eds201$(OBJ_EXT) \
edscfg$(OBJ_EXT) \
ehandler$(OBJ_EXT) \
fstring$(OBJ_EXT) \
futils$(OBJ_EXT) \
gpersist$(OBJ_EXT) \
gthreadt$(OBJ_EXT) \
gxbtree$(OBJ_EXT) \
gxcond$(OBJ_EXT) \
gxconfig$(OBJ_EXT) \
gxcrc32$(OBJ_EXT) \
gxdatagm$(OBJ_EXT) \
gxdbase$(OBJ_EXT) \
gxderror$(OBJ_EXT) \
gxdfp64$(OBJ_EXT) \
gxdfptr$(OBJ_EXT) \
gxdlcode$(OBJ_EXT) \
gxdstats$(OBJ_EXT) \
gxfloat$(OBJ_EXT) \
gxint16$(OBJ_EXT) \
gxint32$(OBJ_EXT) \
gxint64$(OBJ_EXT) \
gxip32$(OBJ_EXT) \
gxlistb$(OBJ_EXT) \
gxmac48$(OBJ_EXT) \
gxmutex$(OBJ_EXT) \
gxrdbdef$(OBJ_EXT) \
gxrdbhdr$(OBJ_EXT) \
gxrdbms$(OBJ_EXT) \
gxrdbsql$(OBJ_EXT) \
gxscomm$(OBJ_EXT) \
gxsema$(OBJ_EXT) \
gxsftp$(OBJ_EXT) \
gxshtml$(OBJ_EXT) \
gxshttp$(OBJ_EXT) \
gxshttpc$(OBJ_EXT) \
gxsmtp$(OBJ_EXT) \
gxsocket$(OBJ_EXT) \
gxsping$(OBJ_EXT) \
gxspop3$(OBJ_EXT) \
gxstream$(OBJ_EXT) \
gxsurl$(OBJ_EXT) \
gxsutils$(OBJ_EXT) \
gxs_b64$(OBJ_EXT) \
gxtelnet$(OBJ_EXT) \
gxthread$(OBJ_EXT) \
gxuint16$(OBJ_EXT) \
gxuint32$(OBJ_EXT) \
gxuint64$(OBJ_EXT) \
htmldrv$(OBJ_EXT) \
httpgrab$(OBJ_EXT) \
leaktest$(OBJ_EXT) \
logfile$(OBJ_EXT) \
memblock$(OBJ_EXT) \
membuf$(OBJ_EXT) \
ostrbase$(OBJ_EXT) \
pod$(OBJ_EXT) \
pscript$(OBJ_EXT) \
scomserv$(OBJ_EXT) \
sreg101$(OBJ_EXT) \
stdafx$(OBJ_EXT) \
strutil$(OBJ_EXT) \
systime$(OBJ_EXT) \
terminal$(OBJ_EXT) \
thelpers$(OBJ_EXT) \
thrapiw$(OBJ_EXT) \
thrpool$(OBJ_EXT) \
ustring$(OBJ_EXT) \
wserror$(OBJ_EXT)
# ===============================================================
