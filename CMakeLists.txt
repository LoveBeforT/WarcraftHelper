cmake_minimum_required(VERSION 3.10.2)

project(WarcraftHelper)

if(!WIN32)
	message(FATAL_ERROR "Only Windows 32-bit is supported")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Only win32 is supported, maybe you've forgot to specify a platform")
endif()

add_subdirectory(WarcraftHelper)
add_subdirectory(WHLoader)
