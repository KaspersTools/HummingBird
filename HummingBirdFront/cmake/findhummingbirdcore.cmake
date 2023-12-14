# FindHummingBirdCore.cmake

# This script locates the HummingBirdCore library

# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Provide a hint for where to look for the HummingBirdCore library
set(HummingBirdCore_HINTS "" CACHE PATH "An optional hint to a directory where HummingBirdCore might be located")

# Look for the header file
find_path(HummingBirdCore_INCLUDE_DIR
        NAMES Application.h
        HINTS ${HummingBirdCore_HINTS}
        PATH_SUFFIXES HummingBirdCore/include
)

# Look for the library
find_library(HummingBirdCore_LIBRARY
        NAMES HUMMINGBIRDCORE
        HINTS ${HummingBirdCore_HINTS}
        PATH_SUFFIXES lib
)

# Handle the QUIETLY and REQUIRED arguments and set the HummingBirdCore_FOUND variable
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HummingBirdCore DEFAULT_MSG
        HummingBirdCore_LIBRARY HummingBirdCore_INCLUDE_DIR)

# If the library has been found, set the HummingBirdCore_LIBRARIES and HummingBirdCore_INCLUDE_DIRS variables
if(HummingBirdCore_FOUND)
    set(HummingBirdCore_LIBRARIES ${HummingBirdCore_LIBRARY})
    set(HummingBirdCore_INCLUDE_DIRS ${HummingBirdCore_INCLUDE_DIR})
endif()

# End of the FindHummingBirdCore.cmake file
