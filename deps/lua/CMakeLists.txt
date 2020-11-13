################################################################
# lua
################################################################
cmake_minimum_required(VERSION 2.6)
project(lua)

set(CORE_H
	src/lapi.h
	src/lctype.h
	src/ldebug.h
	src/ldo.h 
	src/lfunc.h 
	src/lgc.h 
	src/lmem.h 
	src/lobject.h 
	src/lopcodes.h 
	src/lstate.h 
	src/lstring.h 
	src/ltable.h 
	src/ltm.h 
	src/lvm.h
	src/lzio.h
	src/lcode.h
	src/llex.h 
	src/llimits.h
	src/lparser.h 
	src/lundump.h 
	src/lua.h
	src/luaconf.h
	)
set(CORE_C
	src/lctype.c 
	src/ldebug.c 
	src/ldo.c 
	src/lfunc.c 
	src/lgc.c 
	src/lmem.c 
	src/lobject.c 
	src/lopcodes.c 
	src/lstate.c 
	src/lstring.c 
	src/ltable.c 
	src/ltm.c 
	src/lvm.c 
	src/lzio.c 
	src/lcode.c 
	src/lapi.c 
	src/lundump.c 
	src/lparser.c 
	src/ldump.c 
	src/llex.c
	)
set(LIBS 
	src/lualib.h
	src/lauxlib.h
	src/lauxlib.c 
	src/lbaselib.c 
	src/lcorolib.c 
	src/ldblib.c 
	src/liolib.c 
	src/lmathlib.c 
	src/loslib.c 
	src/lstrlib.c 
	src/ltablib.c 
	src/loadlib.c 
	src/linit.c
    src/lutf8lib.c)
source_group(core_h FILES ${CORE_H})
source_group(core_c FILES ${CORE_C})
source_group(libs FILES ${LIBS})
add_library(liblua STATIC ${CORE_H} ${CORE_C} ${LIBS})
add_executable(lua src/lua.c)
target_link_libraries(lua liblua)
add_executable(luac src/luac.c)
target_link_libraries(luac liblua)