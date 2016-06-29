cmake_minimum_required(VERSION 2.8)

project(prof CXX)

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

if(NOT WIN32)
	SET(CMAKE_SYSTEM_NAME Linux)
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_CXX_COMPILER g++)
endif()

#set(DEBUG_MODE OFF)
#set(CMAKE_BUILD_TYPE Release)
    
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

#set install path
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/output")
SET(CMAKE_INSTALL_RPATH .:"${CMAKE_INSTALL_PREFIX}/lib")

message("==========configure info============" )
message("platform is ${HOST_TYPE}" )
message("ar is ${CMAKE_AR}")
message("c compiler is ${CMAKE_C_COMPILER}")
message("c++ compiler is ${CMAKE_CXX_COMPILER}")
message("strip is ${CMAKE_STRIP}")
message("==========configure info============" )

include_directories(${PROJECT_SOURCE_DIR}) 
link_directories(${PROJECT_BINARY_DIR})

set(BUILD_SHARED_LIBS OFF)
#SET( CMAKE_VERBOSE_MAKEFILE on )
set(LibrariesAlwaysUsed "stdc++")
	
if(NOT WIN32)	
	#add_definitions("-pipe -O2 -std=c++11 -Wall")
	add_definitions(" -std=c++11 -pipe -g -finstrument-functions -rdynamic -fpermissive ")
endif()
	
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "No support in-place compiling while using qt_build().")
endif()

include_directories(. ${CMAKE_SOURCE_DIR}/include)
add_definitions(" -include ${CMAKE_SOURCE_DIR}/include/globe_inc.h ")
add_definitions(" -include ${CMAKE_SOURCE_DIR}/include/featrue.h ")
add_definitions(" -include ${CMAKE_SOURCE_DIR}/include/define.h ")

message("Install path: ${CMAKE_INSTALL_RPATH}")
message("Runtime path: ${RUNTIME_BIN_PATH}")
message("install prefix: ${CMAKE_INSTALL_PREFIX}")

option(ENABLE_QT "qt库"  OFF)
option(ENABLE_BOOST "boost库"  OFF)

include(cmake/define.cmake)
include(cmake/static.cmake)

#objdump -x executable_or_lib.so|grep RPATH
#add_subdirectory(prof)
add_subdirectory(prof_parse)
