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

find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
        HINTS
        ENV SDL2IMAGEDIR
        ENV SDL2DIR
        ${SDL2_DIR}
        "/usr/local/Cellar/sdl2_image/2.8.0"
        PATH_SUFFIXES SDL2
        # path suffixes to search inside ENV{SDL2DIR}
        include/SDL2 include
)

# Find MySQL Connector/C++
#find_package(MySQL REQUIRED)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Find libvncserver
find_package(libvncserver REQUIRED)
#include_directories(/usr/local/include)
#link_directories(/usr/local/lib)

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
        src/Terminal/TerminalWindow.cpp
        src/Terminal/TerminalWindow.h
        src/UIWindows/Themes/Themes.h
        src/UIWindows/Themes/ThemeManager.cpp
        src/UIWindows/Themes/ThemeManager.h
        src/Log.cpp
        src/Log.h
        src/stb_image.h
        src/Rendering/Texture.cpp
        src/Rendering/Texture.h
        src/Security/LogInWindow.cpp
        src/Security/LogInWindow.h
        src/Security/LoginManager.cpp
        src/Security/LoginManager.h
        src/Sql/SqlConnection.cpp
        src/Sql/SqlConnection.h
        src/Sql/SqlExample.h
        src/Sql/SqlManager.cpp
        src/Sql/SqlManager.h
)

# Include directories
#include_directories(vendor/imgui/ vendor/glad/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

#imgui
include_directories(vendor/imgui/)
#glad
include_directories(vendor/glad/include)
#Include MySQL Connector/C++ headers
#include_directories(${MYSQL_INCLUDE_DIR})

include_directories(/usr/local/Cellar/mysql/8.2.0_1/include/)
link_directories(/usr/local/Cellar/mysql/8.2.0_1/lib)


add_executable(KBTools ${SOURCES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

target_include_directories(
        KBTools SYSTEM PRIVATE
        ${SPDLOG_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR}
)

target_link_libraries(
        KBTools PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        mysqlclient
#        ${MYSQL_LIBRARY}
)

set_target_properties(
        KBTools PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

