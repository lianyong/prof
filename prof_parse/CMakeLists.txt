
include_directories(. ../prof)

set(target_name prof_parse)

aux_source_directory(. SRC_FILES)
add_executable(${target_name} ${SRC_FILES})

set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "prof_parse_exe")

SET_TARGET_PROPERTIES(${target_name} PROPERTIES VERSION 1.0 SOVERSION 1)

target_link_libraries(${target_name} stdc++ dl)
target_link_libraries(${target_name} com_api boost_program_options)

install(TARGETS ${target_name} LIBRARY DESTINATION lib
                      ARCHIVE DESTINATION lib
                      RUNTIME DESTINATION bin )
					  