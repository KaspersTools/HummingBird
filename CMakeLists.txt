#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_OUTPUT_DIRECTORY}/lib)

cmake_minimum_required(VERSION 3.26)
project(HUMMINGBIRD)

set(CMAKE_CXX_STANDARD 23)

#set cmake build to debug
set(CMAKE_BUILD_TYPE Debug)

#DISPLAY ALL INFO HERE ABOUT THE BUILD
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_OUTPUT_DIRECTORY: ${CMAKE_CURRENT_OUTPUT_DIRECTORY}")

set(HUMMINGBIRD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HUMMINGBIRDCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/HummingbirdCore)
set(HUMMINGBIRDFRONT_DIR ${HUMMINGBIRD_DIR}/HummingbirdFront)

########################################################################
set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( vendor/sdl sdl)

# fmt
add_subdirectory(${HUMMINGBIRD_DIR}/vendor/fmt)

#glad
add_library(glad STATIC
        ${HUMMINGBIRD_DIR}/vendor/glad/src/glad.c
)

#spdlog
add_subdirectory(${HUMMINGBIRD_DIR}/vendor/spdlog)

#imgui
add_library(imgui STATIC
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



#link sdl to imgui
target_link_libraries(imgui
 SDL2-static
)

########################################################################
# Creating the executable and libraries
# HummingbirdCore
add_library(HummingbirdCore STATIC
        HummingBirdCore/src/Application.cpp
        HummingBirdCore/src/Application.h
        HummingBirdCore/src/Log.cpp
        HummingBirdCore/src/Log.h
        HummingBirdCore/src/Terminal/TerminalWindow.cpp
        HummingBirdCore/src/Terminal/TerminalWindow.h
        HummingBirdCore/src/UIWindows/Themes/Themes.h
        HummingBirdCore/src/UIWindows/Themes/ThemeManager.cpp
        HummingBirdCore/src/UIWindows/Themes/ThemeManager.h
        HummingBirdCore/src/stb_image.h
        HummingBirdCore/src/Rendering/Texture.cpp
        HummingBirdCore/src/Rendering/Texture.h
        HummingBirdCore/src/Security/LogInWindow.cpp
        HummingBirdCore/src/Security/LogInWindow.h
        HummingBirdCore/src/Security/LoginManager.cpp
        HummingBirdCore/src/Security/LoginManager.h
        HummingBirdCore/src/Sql/SqlConnection.cpp
        HummingBirdCore/src/Sql/SqlConnection.h
        HummingBirdCore/src/Sql/SqlExample.h
        HummingBirdCore/src/Sql/SqlManager.cpp
        HummingBirdCore/src/Sql/SqlManager.h
        HummingBirdCore/src/GeneralTools/FileDialog.h
        HummingBirdCore/src/GeneralTools/TextEditting/TextEditorWindow.h
        HummingBirdCore/src/GeneralTools/TextEditting/TextEditorWindow.cpp
        HummingBirdCore/src/System/Hosts/EditHostsWindow.h
        HummingBirdCore/src/System/Hosts/EditHostsWindow.cpp
        HummingBirdCore/src/Utils/UUID.h
        HummingBirdCore/src/ApplicationObject.h
        HummingBirdCore/src/UIWindows/ContentExplorer.cpp
        HummingBirdCore/src/UIWindows/ContentExplorer.h
        HummingBirdCore/src/Folder.h
        HummingBirdCore/src/UIWindows/DebugWindow.h
)

# HummingbirdApp
add_executable(HummingbirdApp
        ${HUMMINGBIRDFRONT_DIR}/main.cpp
)

########################################################################
# Optional libraries
option(WITHHUMMINGBIRDKASPERSPECIFIC "WITH HummingBirdKasperSpecific" OFF)
if (WITHHUMMINGBIRDKASPERSPECIFIC)
    message("Building with Kasperspecific")
    set(HUMMINGBIRDKASPERSPECIFIC_DIR ${HUMMINGBIRD_DIR}/HummingBirdKasperSpecific)
    set(HUMMINGBIRDKASPERSPECIFICFILES
        ../HummingBirdKasperSpecific/src/HelloLibrary.cpp
        ../HummingBirdKasperSpecific/src/HelloLibrary.h
        ../HummingBirdKasperSpecific/src/Hellebrekers/VisualLinkLauncher.cpp
        ../HummingBirdKasperSpecific/src/Hellebrekers/VisualLinkLauncher.h
            HummingBirdCore/src/Utils/Input.h
    )

        add_library(
        KasperSpecificTools STATIC
        ${HUMMINGBIRDKASPERSPECIFICFILES}
    )

    #set cpp def
    add_definitions(-DWITHHUMMINGBIRDKASPERSPECIFIC)
endif()

########################################################################
# Include directories
########################################################################
# Include directories
include_directories(
        HummingBirdCore/src
        ${HUMMINGBIRD_DIR}/vendor
        ${HUMMINGBIRD_DIR}/vendor/fmt/include
        ${HUMMINGBIRD_DIR}/vendor/spdlog/include
        ${HUMMINGBIRD_DIR}/vendor/glad/include
        ${HUMMINGBIRD_DIR}/vendor/imgui
        ${HUMMINGBIRD_DIR}/vendor/sdl/include
)

# Include directories optional
if(${WITHHUMMINGBIRDKASPERSPECIFIC})
    include_directories(
        ../HummingBirdKasperSpecific/src
    )
endif ()


########################################################################
# Base Link libraries
target_link_libraries(HummingbirdCore
        SDL2-static
        ${SECURITY_FRAMEWORK}
        fmt::fmt
        spdlog::spdlog
        glad
        imgui
)

target_link_libraries(HummingbirdApp
        HummingbirdCore
        SDL2-static
        ${SECURITY_FRAMEWORK}
        fmt::fmt
        spdlog::spdlog
        glad
        imgui
)

# Optional link libraries
if(${WITHHUMMINGBIRDKASPERSPECIFIC})
    target_link_libraries(HummingbirdCore
        KasperSpecificTools
    )
    target_link_libraries(HummingbirdApp
        KasperSpecificTools
    )
endif ()

########################################################################
# Copying resources from source to build
message("Copying resources from ${CMAKE_CURRENT_SOURCE_DIR}/Assets to ${CMAKE_CURRENT_BINARY_DIR}/Assets")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})