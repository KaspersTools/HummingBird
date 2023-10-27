cmake_minimum_required(VERSION 3.10)
project(imgui_mac_app)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

find_package(glfw3 3.3 REQUIRED)

# glad sources
set(GLAD_SOURCES vendor/glad/src/glad.c)

# ImGui
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        src/terminal_helper_example.h
)


set(SOURCES
    src/main.cpp
    src/terminal_helper_example.h)

include_directories(vendor/imterm/include)

# Include spdlog
add_subdirectory(vendor/spdlog)
include_directories(vendor/spdlog/include)

# Include fmt
add_subdirectory(vendor/fmt)
include_directories(vendor/fmt/include)

# include glad
include_directories(vendor/glad/include)

#include imgui
include_directories(vendor/imgui)


add_executable(imgui_mac_app ${SOURCES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

target_link_libraries(imgui_mac_app glfw spdlog::spdlog fmt::fmt)
target_include_directories(imgui_mac_app PRIVATE vendor/imterm)
add_library(ImTerm INTERFACE)
add_library(ImTerm::terminal ALIAS ImTerm)