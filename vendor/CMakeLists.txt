#cmake_minimum_required(VERSION 3.27)
#project(HUMMINGBIRD)
#
#set(CMAKE_CXX_STANDARD 23)
#
##check if there is an cmake build type
#if (NOT CMAKE_BUILD_TYPE)
#  message("No build type selected, defaulting to Release")
#  set(CMAKE_BUILD_TYPE Release)
#endif ()
#
#if (${CMAKE_BUILD_TYPE} STREQUAL "ASAN")
#  message("Building with ASAN")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=pointer-compare -g")
#  set(CMAKE_BUILD_TYPE ASAN)
#endif ()
#
#
#add_executable(HummingBirdCore)
#
#add_subdirectory("HummingBirdCore")
#add_subdirectory("vendor")


#NEW LIST |

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../cmake/)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# SDL
# https://github.com/libsdl-org/SDL
set(SDL_LIBC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(sdl)
set(SDL2_FOUND ON CACHE BOOL "" FORCE)

# imgui
set(KDB_IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(imgui)
target_link_libraries(HummingBirdCore KDB_IMGUI-STATIC)

#link imgui to sdl and make this public
target_link_libraries(KDB_IMGUI-STATIC PUBLIC SDL2::SDL2main SDL2::SDL2-static)
target_include_directories(KDB_IMGUI-STATIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sdl/include)

#fmt
IF (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/fmt)
    message("fmt folder not found")
ELSE ()
    message("fmt folder found")
ENDIF ()
add_subdirectory(fmt)
target_link_libraries(HummingBirdCore fmt::fmt)

#spdlog
IF(NOT EXISTS spdlog)
    message("spdlog folder not found")
ELSE()
    message("spdlog folder found")
ENDIF()
add_subdirectory(spdlog)
target_link_libraries(HummingBirdCore spdlog::spdlog)

##################system##################
#mysql
find_package(MySQL)
if (NOT MYSQL_FOUND)
    message(WARNING "MySQL not found")
else ()
    message("MySQL found")
endif ()

target_link_libraries(HummingBirdCore ${MYSQL_LIBRARIES})
target_include_directories(HummingBirdCore PRIVATE ${MYSQL_INCLUDE_DIR})

# get Security
find_library(SECURITY_FRAMEWORK Security)
if (NOT SECURITY_FRAMEWORK)
    message(FATAL_ERROR "Security not found Are you on mac?")
else ()
endif ()
target_link_libraries(HummingBirdCore
        ${SECURITY_FRAMEWORK}
)

# get IOKit
find_library(IOKit IOKit)
if (NOT IOKit)
    message(FATAL_ERROR "IOKit not found Are you on mac?")
else ()
    message("IOKit found")
endif ()
target_link_libraries(HummingBirdCore
        ${IOKit}
)

# get CoreFoundation
find_library(CoreFoundation CoreFoundation)
if (NOT CoreFoundation)
    message(FATAL_ERROR "CoreFoundation not found Are you on mac?")
else ()
    message("CoreFoundation found")
endif ()
target_link_libraries(HummingBirdCore
        ${CoreFoundation}
)

# Find the System Configuration framework
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if (NOT SYSTEM_CONFIGURATION_FRAMEWORK)
    message(FATAL_ERROR "SystemConfiguration not found Are you on mac?")
else ()
    message("SystemConfiguration found")
endif ()
target_link_libraries(HummingBirdCore
        ${SYSTEM_CONFIGURATION_FRAMEWORK}
)

