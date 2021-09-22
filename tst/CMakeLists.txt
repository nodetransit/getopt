cmake_minimum_required (VERSION 3.16)
project(getopt_demo)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

add_subdirectory("./../src" .getopt)

#---------------------------------------------------------------------

option (DEBUG "Compile with debug symbols." OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG ON)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set (DEBUGINFO ON)
endif ()

option (VERBOSE "Verbose console output." OFF)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()

#---------------------------------------------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif()

if (WIN32) # (or LOSE)
    set (LOSE ON)
endif ()

if (APPLE)
    set (BANANA ON)
endif()

#---------------------------------------------------------------------

macro(add_example name)
    add_executable(${name} ${name}.cpp)
    set_property (TARGET ${name} PROPERTY CXX_STANDARD 11)

    if (NOT MSVC)
        add_definitions("--pedantic")

        target_compile_options (${name} PRIVATE "-Wno-missing-braces"
                                                "-Wall")

        if (DEBUG)
            target_compile_options (${name} PRIVATE #"-Wno-bool-compare"
                                                    #"-Wno-incompatible-pointer-types"
                                                    "-Wno-unused-parameter"
                                                    #"-Wno-unused-variable"
                                                    "-Wno-unused-function"
                                                    "-Wno-format")
        endif ()

    endif ()

    if (DEBUG)
        message (STATUS "Compiling with DEBUG symbols")

        if (NOT MSVC)
            add_definitions (-g
                             #-Wall
                             #-Werror
                             #-Wfatal-errors
                             -fdiagnostics-show-option
                             -fmax-errors=3)
        else ()
            add_definitions ("-Zi")
        endif ()
    else ()
        if (NOT MSVC)
            target_compile_options (${name} PRIVATE "-O3"
                                                    "-c"
                                                    "-s")
        else ()
            target_compile_options (${name} PRIVATE "-Ox"
                                                    "-GF"
                                                    "-c"
                                                    "-W2"
                                                    "-GL")
        endif ()
    endif ()

    target_link_libraries(${name} "getopt")
endmacro()

#---------------------------------------------------------------------

add_example("demo1")
add_example("demo2")
