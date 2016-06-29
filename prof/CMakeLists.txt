
include_directories(.)

set(target_name prof)

aux_source_directory(. SRC_FILES)
add_executable(${target_name} ${SRC_FILES})

target_link_libraries(${target_name} stdc++ dl)

install(TARGETS ${target_name} LIBRARY DESTINATION lib
                      ARCHIVE DESTINATION lib
                      RUNTIME DESTINATION bin )
					  