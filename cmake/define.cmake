macro(DEEP_STRIP target)
    if((NOT DEBUG_MODE) AND ENABLE_STRIP)
        get_target_property(TARGET_TYPE ${target} TYPE)
        #使用Backtrace时，不对可执行程序strip
        if(NOT (ENABLE_BACKTRACE AND ("${TARGET_TYPE}" STREQUAL "EXECUTABLE")))
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${BUILDTOOL_PREFIX}-strip -x --strip-unneeded -R .pdr -R .comment -R .note -R .mdebug.abi32 $<TARGET_FILE:${target}>
                COMMENT "Deep strip ${target}."
            )
        endif()
    endif()
endmacro()


if(ENABLE_QT)
    find_package(Qt4 REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DIS_QT_BUILD=1)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    if(DEBUG_MODE)
        set(PCH_DEBUG_FLAG "-DQT_DEBUG")
    else()
        set(PCH_DEBUG_FLAG "-DQT_NO_DEBUG")
    endif()
    #precompile_header(qtheaders.h "-fPIC ${PCH_DEBUG_FLAG}")
    dirctory_use_pch(.)
endif()

if(ENABLE_BOOST)	
	set(BOOST_USE_STATIC_LIBS ON)
	find_package(Boost REQUIRED COMPONENTS program_options filesystem system date_time)
	include_directories(${BOOST_INCLUDE_DIR})
	link_directories(${BOOST_LIBRARY_DIR})
endif()

