include_directories(.)

aux_source_directory(. SRC_FILES)

add_library(function OBJECT ${SRC_FILES})
	
#FILE(GLOB headers *.h)
#INSTALL(FILES ${headers} DESTINATION include)