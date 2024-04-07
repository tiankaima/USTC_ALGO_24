# configure to run all *.c and *.cpp files as individual executables, no linking required:
cmake_minimum_required(VERSION 3.10)
project(ustc_algo_24)

set(CMAKE_CXX_STANDARD 20)

# Find all .c and .cpp files in subdirectories
file(GLOB_RECURSE SOURCES
        RELATIVE ${CMAKE_SOURCE_DIR}
        CONFIGURE_DEPENDS
        "*.cpp"
        "*.c")

foreach (SOURCE_FILE ${ALL_SOURCES})
    string(FIND ${SOURCE_FILE} "build/" BUILD_DIR_POS)
    string(FIND ${SOURCE_FILE} "cmake-build-debug/" CMAKE_BUILD_DEBUG_DIR_POS)
    if (BUILD_DIR_POS EQUAL -1 AND CMAKE_BUILD_DEBUG_DIR_POS EQUAL -1)
        list(APPEND SOURCES ${SOURCE_FILE})
    endif ()
endforeach ()

# For each source file, create an executable
foreach (SOURCE ${SOURCES})
    # Get a valid name for the executable by replacing non-alphanumeric characters in file paths
    string(REGEX REPLACE "[^A-Za-z0-9]+" "_" EXECUTABLE_NAME ${SOURCE})
    # Exclude the file extension for the executable name
    string(REPLACE ".cpp" "" EXECUTABLE_NAME ${EXECUTABLE_NAME})
    string(REPLACE ".c" "" EXECUTABLE_NAME ${EXECUTABLE_NAME})
    # Create an executable from the source file
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
endforeach ()