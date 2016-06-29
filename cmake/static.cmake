
add_subdirectory(function)
add_subdirectory(compress)

set(target_name com_api)

add_library(${target_name} STATIC 
	$<TARGET_OBJECTS:function>
	$<TARGET_OBJECTS:compress>
	)	
	
SET_TARGET_PROPERTIES(${target_name} PROPERTIES VERSION 1.0 SOVERSION 1)	
			  
install(TARGETS ${target_name} LIBRARY DESTINATION lib
                      ARCHIVE DESTINATION lib
                      RUNTIME DESTINATION bin)
