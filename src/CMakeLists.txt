cmake_minimum_required (VERSION 3.16)
project (getopt
         VERSION 1.0.0
         DESCRIPTION "Simple getopt replacement class (C++11)")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

include (CheckIncludeFiles)
include (CheckIncludeFileCXX)

set (CMAKE_CXX_STANDARD 11)

string (REPLACE "." "," PRODUCT_FILE_VERSION ${PROJECT_VERSION})

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

option (GETOPT_EXPORT_LIB "Build Shared Libraries." ON)
set (GETOPT_EXPORT_SHARED OFF)
set (GETOPT_EXPORT_STATIC OFF)
if (GETOPT_EXPORT_LIB)
    set (GETOPT_LIB_TYPE SHARED)
    set (GETOPT_EXPORT_SHARED ON)
else ()
    set (GETOPT_LIB_TYPE STATIC)
    set (GETOPT_EXPORT_STATIC ON)
endif ()

#---------------------------------------------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif ()

if (WIN32) # (or LOSE)
    set (LOSE ON)
endif ()

if (APPLE)
    set (BANANA ON)
endif()

if (LINUX)
    check_include_file_cxx ("unistd.h" HAVE_UNISTD)
    if (NOT HAVE_UNISTD)
        message (FATAL_ERROR "cannot find unistd.h")
    endif ()
    check_include_file_cxx ("stdio.h" HAVE_STDIO)
    if (NOT HAVE_STDIO)
        message (FATAL_ERROR "cannot find stdio.h")
    endif ()
    check_include_file_cxx ("stdlib.h" HAVE_STDLIB)
    if (NOT HAVE_STDLIB)
        message (FATAL_ERROR "cannot find stdlib.h")
    endif ()
endif ()

if (LOSE)
    check_include_file_cxx ("io.h" HAVE_IO)
    if (NOT HAVE_IO)
        message (FATAL_ERROR "cannot find io.h")
    endif ()
    check_include_file_cxx ("shellapi.h" HAVE_SHELLAPI)
    if (NOT HAVE_SHELLAPI)
        message (FATAL_ERROR "cannot find shellapi.h")
    endif ()
    check_include_file_cxx ("winsock2.h" HAVE_WINSOCK2)
    if (NOT HAVE_WINSOCK2)
        message (FATAL_ERROR "cannot find winsock2.h")
    endif ()
endif ()


if (LOSE)
    if (MSVC)
        set (DLLTOOL OFF)
    else ()
        message (STATUS "Detecting dlltool")
        find_program (DLLTOOL dlltool)
        if (DLLTOOL)
            message (STATUS "  Found dlltool: ${DLLTOOL}")
        else ()
            message (WARNING "dlltool not found. Skipping import library generation.")
        endif ()
    endif ()

    if (MINGW)
        set (CMAKE_RC_COMPILER_INIT windres)
        enable_language (RC)
        set (CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <DEFINES> -o <OBJECT> -i <SOURCE>")
    endif ()

endif ()

#---------------------------------------------------------------------

set (SOURCE_FILES "getopt.cpp"
                  )
if (LOSE)
    list (APPEND SOURCE_FILES "manifest.rc")
endif ()

if (VERBOSE)
    message(STATUS "${CMAKE_PROJECT_NAME} files:")
    foreach (_variableName ${SOURCE_FILES})
        message(STATUS "  - ${_variableName}")
    endforeach()
endif ()

set (BINARY_NAME "getopt")
add_library (${BINARY_NAME} ${GETOPT_LIB_TYPE} ${SOURCE_FILES})

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/getopt.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/getopt.hpp")
#configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/common.hpp.in"            "${CMAKE_CURRENT_SOURCE_DIR}/common.hpp")
#if (LOSE)
#    configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/manifest.rc.in" "${CMAKE_CURRENT_SOURCE_DIR}/manifest.rc")
#endif ()

#---------------------------------------------------------------------

set (COMMON_COMPILE_OPTIONS "")
set (COMMON_DEBUG_COMPILE_OPTTIONS "")

if (NOT MSVC)
    list (APPEND COMMON_COMPILE_OPTIONS "-Wno-missing-braces"
                                        "-Wall"
                                        "-Wextra"
                                        #-Wfatal-errors
                                        "-Wno-missing-field-initializers"
                                        "-fno-strict-aliasing")
    #add_definitions ("--pedantic"
    #                 "-ansi")

    if (DEBUG)
        list (APPEND COMMON_DEBUG_COMPILE_OPTTIONS #"-Wno-bool-compare"
                                                   #"-Wno-incompatible-pointer-types"
                                                   "-Wno-unused-parameter"
                                                   "-Wno-sign-compare"
                                                   #"-Wno-unused-variable"
                                                   "-Wno-unused-function"
                                                   "-Wno-format")
    endif ()
else ()
    list (APPEND COMMON_COMPILE_OPTIONS "-GR"
          )
endif ()

if (DEBUG OR DEBUGINFO)
    message (STATUS "Compiling with DEBUG symbols")

    if (NOT MSVC)
        add_definitions (-g3
                         #-gz=zlib
                         -fdiagnostics-show-option
                         -femit-class-debug-always
                         -fvar-tracking
                         -fmax-errors=3)
    else ()
        list (APPEND COMMON_COMPILE_OPTIONS "-Zi")
    endif ()
endif ()

if (NOT DEBUG)
    message (STATUS "Compiling with optimizations")

    if (NOT MSVC)
        list (APPEND COMMON_COMPILE_OPTIONS "-O3"
                                            "-s"
                                            "-D_FORTIFY_SOURCE=2")
    else ()
        list (APPEND COMMON_COMPILE_OPTIONS #"-Ox"
                                            "-GF"
                                            "-GL"
                                            "-LTCG"
                                             )
    endif ()
endif ()

#---------------------------------------------------------------------

if (${GETOPT_LIB_TYPE} STREQUAL "STATIC")
    set_property(TARGET ${BINARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
    #add_definitions (-DGETOPT_LIB_EXPORT=1
    #                 )
elseif (${GETOPT_LIB_TYPE} STREQUAL "SHARED")
    if (LOSE)
        set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

        if (MSVC)
            set_target_properties (${BINARY_NAME} PROPERTIES
                                                  LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} \/DYNAMICBASE")
        endif ()

        if (DLLTOOL)
            set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
                                                  ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
                                                  LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} -Wl,--output-def=lib${BINARY_NAME}.def")

            add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
                               #WORKING_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
                               COMMAND echo "Generating import library"
                               COMMAND "${DLLTOOL}" --dllname "${BINARY_NAME}.dll"
                                                    --input-def "${BINARY_NAME}.def"
                                                    --output-lib "${BINARY_NAME}.lib"
                                                    #todo add --as-flags=--64
                                                    )

        endif ()
    else ()
        set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
                                                         ARCHIVE_OUTPUT_NAME "${BINARY_NAME}")
    endif ()

    #if (LINUX)
    #    set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
    #                           ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
    #                           LINK_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT}
    #                                      "-Wl,-rpath=./")
    #
    #endif ()
    list (APPEND COMMON_COMPILE_OPTIONS -DGETOPT_LIB_EXPORT=1
          )

    set_target_properties(${BINARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(${BINARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
endif ()

target_compile_options (${BINARY_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS}
                        ${COMMON_DEBUG_COMPILE_OPTTIONS})
set_property (TARGET ${BINARY_NAME} PROPERTY CXX_STANDARD 11)

target_include_directories(${BINARY_NAME}
                           PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                  "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                  )

#---------------------------------------------------------------------

