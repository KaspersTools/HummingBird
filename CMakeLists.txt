cmake_minimum_required(VERSION 3.12)
project(KBTools)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#basic libraries
include(cmake/common.cmake)
include(cmake/fmt.cmake)
include(cmake/spdlog.cmake)
include(cmake/config.cmake)
include(cmake/opengl.cmake)

#Find glfw
find_package(glfw3 3.3 REQUIRED)

# glad sources
set(GLAD_SOURCES vendor/glad/src/glad.c)

set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_internal.h
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
)

set(SOURCES
        src/main.cpp
        src/Application.cpp
        src/Application.h
        src/UIWindows/TerminalWindow.cpp
        src/UIWindows/TerminalWindow.h
)

#include imgui
include_directories(vendor/imgui/)
# include glad
include_directories(vendor/glad/include)


add_executable(KBTools ${SOURCES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

target_include_directories(
        KBTools SYSTEM PRIVATE
        ${SPDLOG_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR})

target_link_libraries(
        KBTools PRIVATE
        glfw)

set_target_properties(
        KBTools PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

