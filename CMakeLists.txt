cmake_minimum_required(VERSION 3.26)
project(HUMMINGBIRD)

set(CMAKE_CXX_STANDARD 23)

set(HUMMINGBIRD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HUMMINGBIRDCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/HummingbirdCore)

########################################################################
# SDL2
add_subdirectory(${HUMMINGBIRD_DIR}/vendor/sdl)
set ( SDL2_LIBRARY SDL2 )
# SDL2_image
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# spdlog
add_subdirectory(${HUMMINGBIRD_DIR}/vendor/spdlog)
# fmt
add_subdirectory(${HUMMINGBIRD_DIR}/vendor/fmt)


########################################################################
# Include directories
include_directories(
        ${HUMMINGBIRDCORE_DIR}/src
        ${HUMMINGBIRD_DIR}/vendor
        ${HUMMINGBIRD_DIR}/vendor/fmt/include
        ${HUMMINGBIRD_DIR}/vendor/spdlog/include
        ${HUMMINGBIRD_DIR}/vendor/glad/include
        ${HUMMINGBIRD_DIR}/vendor/imgui
        ${HUMMINGBIRD_DIR}/vendor/sdl/include
)

########################################################################
# Source files
#glad
add_library(glad
    ${HUMMINGBIRD_DIR}/vendor/glad/src/glad.c
)

#imgui
add_library(imgui
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imconfig.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_demo.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_draw.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_internal.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_tables.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_widgets.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_rectpack.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_textedit.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_truetype.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.cpp
    ${HUMMINGBIRD_DIR}/vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.h
    ${HUMMINGBIRD_DIR}/vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.cpp
)

add_library(HummingbirdCore
    ${HUMMINGBIRDCORE_DIR}/src/Application.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Application.h
    ${HUMMINGBIRDCORE_DIR}/src/Log.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Log.h
    ${HUMMINGBIRDCORE_DIR}/src/Terminal/TerminalWindow.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Terminal/TerminalWindow.h
    ${HUMMINGBIRDCORE_DIR}/src/UIWindows/Themes/Themes.h
    ${HUMMINGBIRDCORE_DIR}/src/UIWindows/Themes/ThemeManager.cpp
    ${HUMMINGBIRDCORE_DIR}/src/UIWindows/Themes/ThemeManager.h
    ${HUMMINGBIRDCORE_DIR}/src/stb_image.h
    ${HUMMINGBIRDCORE_DIR}/src/Rendering/Texture.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Rendering/Texture.h
    ${HUMMINGBIRDCORE_DIR}/src/Security/LogInWindow.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Security/LogInWindow.h
    ${HUMMINGBIRDCORE_DIR}/src/Security/LoginManager.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Security/LoginManager.h
    ${HUMMINGBIRDCORE_DIR}/src/Sql/SqlConnection.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Sql/SqlConnection.h
    ${HUMMINGBIRDCORE_DIR}/src/Sql/SqlExample.h
    ${HUMMINGBIRDCORE_DIR}/src/Sql/SqlManager.cpp
    ${HUMMINGBIRDCORE_DIR}/src/Sql/SqlManager.h
    ${HUMMINGBIRDCORE_DIR}/src/GeneralTools/FileDialog.h
    ${HUMMINGBIRDCORE_DIR}/src/GeneralTools/TextEditting/TextEditorWindow.h
    ${HUMMINGBIRDCORE_DIR}/src/GeneralTools/TextEditting/TextEditorWindow.cpp
    ${HUMMINGBIRDCORE_DIR}/src/System/Hosts/EditHostsWindow.h
    ${HUMMINGBIRDCORE_DIR}/src/System/Hosts/EditHostsWindow.cpp
)

add_library(Hummingbird INTERFACE)

set(CMAKE_CURRENT_OUTPUT_DIRECTORY ${HUMMINGBIRD_DIR}/bin/${CMAKE_BUILD_TYPE}/)

set(HUMMINGBIRDFRONT_DIR ${HUMMINGBIRD_DIR}/HummingbirdFront)

add_executable(HummingbirdApp
    ${HUMMINGBIRDFRONT_DIR}/main.cpp
)

target_link_libraries(HummingbirdCore
    ${SDL2_LIBRARY}
    ${SECURITY_FRAMEWORK}
    fmt::fmt
    spdlog::spdlog
    glad
    imgui
)

target_link_libraries(HummingbirdApp
    HummingbirdCore
    ${SDL2_LIBRARY}
    ${SECURITY_FRAMEWORK}
    fmt::fmt
    spdlog::spdlog
    glad
    imgui
)