cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD_PLUGIN_TEST)
set(CMAKE_CXX_STANDARD 23)

set(HUMMINGBIRD_PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(HUMMINGBIRD_PLUGIN_TEST SHARED TestPlugin/TestPlugin.cpp)
target_include_directories(HUMMINGBIRD_PLUGIN_TEST PRIVATE include)
set(PLUGINS_DIR "/Users/k.debruin/_private/_hummingbird/workdir/HummingBird/cmake-build-debug-exe/plugins")
set_target_properties(HUMMINGBIRD_PLUGIN_TEST PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGINS_DIR}/TestPlugin"
)
#check if there is already a imgui target,
IF(NOT TARGET imgui)
    #if not, add it
    add_subdirectory(../vendor/imgui imgui EXCLUDE_FROM_ALL)
ENDIF()

#enable IMGUI_ENABLE_STDLIB_SUPPORT
target_link_libraries(HUMMINGBIRD_PLUGIN_TEST PUBLIC imgui imgui-stdlib)
list(APPEND HUMMINGBIRD_PLUGIN_TEST_LIBS
        imgui
)
#make hummingbird_plugin available to parent cmakelist
