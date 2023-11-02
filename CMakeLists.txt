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


# Find SDL2
find_package(SDL2 REQUIRED)

set(GLAD_SOURCES vendor/glad/src/glad.c)


set(IMGUI_SOURCES

        vendor/imgui/imconfig.h
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui.h
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_internal.h
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imstb_rectpack.h
        vendor/imgui/imstb_textedit.h
        vendor/imgui/imstb_truetype.h
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/backends/imgui_impl_opengl3.h
        vendor/imgui/backends/imgui_impl_sdl2.cpp
        vendor/imgui/backends/imgui_impl_sdlrenderer2.h
        vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp

        #TODO: REPLACE WITH SDL
)

set(SOURCES
        src/main.cpp
        src/Application.cpp
        src/Application.h
        src/UIWindows/TerminalWindow.cpp
        src/UIWindows/TerminalWindow.h
        src/UIWindows/Themes/Themes.h
        src/UIWindows/Themes/ThemeManager.cpp
        src/UIWindows/Themes/ThemeManager.h
        src/Log.cpp
        src/Log.h
)

# Include directories
#include_directories(vendor/imgui/ vendor/glad/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(vendor/imgui/)
include_directories(vendor/glad/include)

add_executable(KBTools ${SOURCES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

target_include_directories(
        KBTools SYSTEM PRIVATE
        ${SPDLOG_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR})

target_link_libraries(
        KBTools PRIVATE
        ${SDL2_LIBRARIES}
)

set_target_properties(
        KBTools PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

