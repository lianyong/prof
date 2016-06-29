include_directories(.)

aux_source_directory(. SRC_FILES)

add_library(compress OBJECT ${SRC_FILES})
	
#FILE(GLOB headers *.h)
#INSTALL(FILES ${headers} DESTINATION include)