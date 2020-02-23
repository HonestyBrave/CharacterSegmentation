PROJECT(CharacterSegmentation)

cmake_minimum_required(VERSION 3.13)

SET(RUNTIME_INSTALL_DIR bin
    CACHE PATH "Install dir for executables and dlls")
SET(ARCHIVE_INSTALL_DIR lib
    CACHE PATH "Install dir for static libraries")
SET(LIBRARY_INSTALL_DIR lib
    CACHE PATH "Install dir for shared libraries")
SET(INCLUDE_INSTALL_DIR include
    CACHE PATH "Install dir for headers")
SET(PACKAGE_INSTALL_DIR lib/cmake
    CACHE PATH "Install dir for cmake package config files")
MARK_AS_ADVANCED( RUNTIME_INSTALL_DIR ARCHIVE_INSTALL_DIR INCLUDE_INSTALL_DIR PACKAGE_INSTALL_DIR )

# This ensures shared DLL are in the same dir as executable on Windows.
# Put all executables / libraries are in a project global directory.
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all static libraries.")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all dynamic libraries on Unix.")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single directory for all executable and dynamic libraries on Windows.")
MARK_AS_ADVANCED( CMAKE_RUNTIME_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY CMAKE_ARCHIVE_OUTPUT_DIRECTORY )

set(CMAKE_DEBUG_POSTFIX "d")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC") 
endif()

add_subdirectory("src")
add_subdirectory("test")
#target_link_libraries(main ${LIB_NAME})

