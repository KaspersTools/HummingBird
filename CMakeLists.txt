cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD)
#TODO: CLEAN UP THIS FUCKING MESS!!!!!!!!!!!!!!!!!!!!!!!!!!
#TODO: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
set(CMAKE_CXX_STANDARD 23)

message(STATUS "------HUMNMING BIRD------")

message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_OUTPUT_DIRECTORY: ${CMAKE_CURRENT_OUTPUT_DIRECTORY}")
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()

set(HUMMINGBIRD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HummingBirdCore_DIR ${CMAKE_CURRENT_SOURCE_DIR}/HummingBirdCore)
set(HUMMINGBIRDFRONT_DIR ${HUMMINGBIRD_DIR}/HummingbirdFront)

# Add cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#options
option(HUMMINGBIRD_EXE "With HummingBirdCore exe" OFF)
option(HUMMINGBIRD_KASPER "With HummingBird Kasper exe" ON)

#check if there is an cmake build type
if (NOT CMAKE_BUILD_TYPE)
  message("No build type selected, defaulting to Release")
  set(CMAKE_BUILD_TYPE Release)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "ASAN")
  message("Building with ASAN")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=pointer-compare -g")
  set(CMAKE_BUILD_TYPE ASAN)
endif ()

#########HUMMINGBIRD
#library
set(HUMMINGBIRDCOURESTATICFILES
        HummingBirdCore/src/stb_image.h
        HummingBirdCore/src/CoreRef.h
        HummingBirdCore/src/Application.cpp
        HummingBirdCore/src/Application.h
        HummingBirdCore/src/Log.cpp
        HummingBirdCore/src/Log.h
        HummingBirdCore/src/Terminal/TerminalWindow.cpp
        HummingBirdCore/src/Terminal/TerminalWindow.h
        HummingBirdCore/src/UIWindows/Themes/Themes.h
        HummingBirdCore/src/UIWindows/Themes/Themes.cpp
        HummingBirdCore/src/UIWindows/Themes/ThemeManager.cpp
        HummingBirdCore/src/UIWindows/Themes/ThemeManager.h
        HummingBirdCore/src/Security/LogInWindow.cpp
        HummingBirdCore/src/Security/LogInWindow.h
        HummingBirdCore/src/Security/LoginManager.cpp
        HummingBirdCore/src/Security/LoginManager.h
        HummingBirdCore/src/System/Hosts/EditHostsWindow.h
        HummingBirdCore/src/System/Hosts/EditHostsWindow.cpp
        HummingBirdCore/src/Utils/UUID.h
        HummingBirdCore/src/ApplicationObject.h
        HummingBirdCore/src/UIWindows/ContentExplorer.cpp
        HummingBirdCore/src/UIWindows/ContentExplorer.h
        HummingBirdCore/src/Folder.h
        HummingBirdCore/src/UIWindows/LogWindow.h
        HummingBirdCore/src/Utils/Input.h
        HummingBirdCore/src/Updatable.h
        HummingBirdCore/src/UIWindows/Widget/DataViewer.h
        HummingBirdCore/src/UIWindows/Widget/MetricsWidget.h
        HummingBirdCore/src/Utils/UIUtils.h
        HummingBirdCore/src/System/LaunchDaemonsManager.cpp
        HummingBirdCore/src/System/LaunchDaemonsManager.h
        HummingBirdCore/src/Utils/Plist/PlistUtil.h
        HummingBirdCore/src/UIWindows/Widget/AllAddonsWidget.h
        HummingBirdCore/src/UIWindows/MainMenuBar.cpp
        HummingBirdCore/src/Sql/SqlWindow.cpp
        HummingBirdCore/src/Sql/SqlWindow.h
)

add_library(HummingBirdCore STATIC
        ${HUMMINGBIRDCOURESTATICFILES}
)

#third party
add_subdirectory(vendor)

target_precompile_headers(HummingBirdCore PUBLIC HummingBirdCore/src/PCH/pch.h)
target_include_directories(HummingBirdCore PUBLIC HummingBirdCore/src)

if (HUMMINGBIRD_EXE)
  message("Building with HummingBirdCore as exe")
  add_executable(HummingBirdCoreExe
          ${HummingBirdCore_DIR}/src/main.cpp
  )
  target_link_libraries(HummingBirdCoreExe HummingBirdCore)
  target_include_directories(HummingBirdCoreExe PRIVATE HummingBirdCore/src)
else ()
  message("Building with HummingBirdCore as lib")
endif ()

set(HUMMINGBIRD_PLUGIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)


target_include_directories(HummingBirdCore PUBLIC HummingBirdPluginManager/include)

option(HUMMINGBIRD_PLUGINS_WITH_MANAGER "With plugin manager" ON)
if(HUMMINGBIRD_PLUGINS_WITH_MANAGER)
  message("Building with plugin manager")
  add_subdirectory(HummingBirdPluginManager EXCLUDE_FROM_ALL)
endif()
option(HUMMINGBIRD_PLUGINS_WITH_EXAMPLE "With plugin example" ON)
if(HUMMINGBIRD_PLUGINS_WITH_EXAMPLE)
  add_subdirectory(HummingBirdPluginExample EXCLUDE_FROM_ALL)
endif()
########################################################################
# Copying resources from source to build
message("Copying resources from ${CMAKE_CURRENT_SOURCE_DIR}/Assets to ${CMAKE_CURRENT_BINARY_DIR}/Assets")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copying plugins from source to build
message("Copying plugins from ${CMAKE_CURRENT_SOURCE_DIR}/plugins to ${CMAKE_CURRENT_BINARY_DIR}/plugins")
