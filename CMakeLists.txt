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

# Add cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

####MAIN PROJECT####
# HummingbirdCore
add_library(HummingbirdCore STATIC
        HummingBirdCore/src/CoreRef.h
        HummingBirdCore/src/Application.cpp
        HummingBirdCore/src/Application.h
        HummingBirdCore/src/Logging/MainLogSink.h
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
        HummingBirdCore/src/System/Hosts/EditHostsWindow.h
        HummingBirdCore/src/System/Hosts/EditHostsWindow.cpp
        HummingBirdCore/src/Utils/UUID.h
        HummingBirdCore/src/ApplicationObject.h
        HummingBirdCore/src/UIWindows/ContentExplorer.cpp
        HummingBirdCore/src/UIWindows/ContentExplorer.h
        HummingBirdCore/src/Folder.h
        HummingBirdCore/src/UIWindows/LogWindow.h
        HummingBirdCore/src/Logging/ImGuiLogSink.h
        HummingBirdCore/src/Utils/Input.h
        HummingBirdCore/src/Updatable.h
        HummingBirdCore/src/System/SysInfo.cpp
        HummingBirdCore/src/System/SysInfo.h
        HummingBirdCore/src/System/SysInfoWindow.cpp
        HummingBirdCore/src/System/SysInfoWindow.h
        HummingBirdCore/src/Sql/SqlWindow.cpp
        HummingBirdCore/src/Sql/SqlWindow.h
        HummingBirdCore/src/Sql/SqlConnection.cpp
        HummingBirdCore/src/Sql/SqlConnection.h
        HummingBirdCore/src/UIWindows/Widget/DataViewer.h
)

# HummingbirdApp
add_executable(HummingbirdApp
        ${HUMMINGBIRDFRONT_DIR}/main.cpp
)

# Link HummingbirdCore to HummingbirdApp
target_link_libraries(HummingbirdApp
        HummingbirdCore
)

# Core include directories
include_directories(
        HummingBirdCore/src
)


########################################################################
# fmt
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/fmt)
    message("fmt folder not found")
else ()
    message("fmt folder found")
    add_subdirectory(${HUMMINGBIRD_DIR}/vendor/fmt)
    #link
    target_link_libraries(HummingbirdCore
            fmt::fmt
    )

    target_link_libraries(HummingbirdApp
            fmt::fmt
    )

    #include
    include_directories(
            ${HUMMINGBIRD_DIR}/vendor/fmt/include
    )
endif ()


#sdl
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/sdl)
    message("SDL folder not found")
else ()
    message("SDL folder found needed by imgui")
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/sdl sdl)

    target_link_libraries(HummingbirdCore
            SDL2-static
    )

    target_link_libraries(HummingbirdApp
            SDL2-static
    )

    #include
    include_directories(
            ${HUMMINGBIRD_DIR}/vendor/sdl/include
    )
endif ()


#glad
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/glad)
    message("Glad folder not found")
else ()
    message("Glad folder found")
    add_library(glad STATIC
            ${HUMMINGBIRD_DIR}/vendor/glad/src/glad.c
    )

    #include glad
    include_directories(
            ${HUMMINGBIRD_DIR}/vendor/glad/include
    )

    # Link glad
    target_link_libraries(HummingbirdCore
            glad
    )

    target_link_libraries(HummingbirdApp
            glad
    )
endif ()


#spdlog
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/spdlog)
    message("spdlog folder not found")
else ()
    message("spdlog folder found")
    add_subdirectory(${HUMMINGBIRD_DIR}/vendor/spdlog)

    #include spdlog
    include_directories(
            ${HUMMINGBIRD_DIR}/vendor/spdlog/include
    )

    # Link spdlog
    target_link_libraries(HummingbirdCore
            spdlog::spdlog
    )

    target_link_libraries(HummingbirdApp
            spdlog::spdlog
    )
endif ()


#imgui
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/imgui)
    message("Imgui folder not found")
else ()
    add_library(imgui STATIC
            #imgui
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/imconfig.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_draw.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_internal.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_tables.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_widgets.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_textedit.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_rectpack.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_truetype.h

            #imgui demos
            ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_demo.cpp

            #backends
            ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
            ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.cpp
            ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.h

            #misc
            ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
    )

    #include imgui
    include_directories(
            ${HUMMINGBIRD_DIR}/vendor/imgui
            ${HUMMINGBIRD_DIR}/vendor/imgui/backends
            ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp
    )

    # Link imgui
    target_link_libraries(HummingbirdCore
            imgui
    )

    target_link_libraries(HummingbirdApp
            imgui
    )

    #link sdl to imgui if sdl is valid
    if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/sdl)
        message("SDL folder not found AND its needed by imgui")
    else ()
        message("SDL folder found")
        target_link_libraries(imgui
                SDL2-static
        )
    endif ()
endif ()


#mysql
find_package(MySQL)
if(NOT MYSQL_FOUND)
    message(WARNING "MySQL not found")
else()
    message("MySQL found")
    target_link_libraries(HummingbirdCore
            ${MYSQL_LIBRARIES}
    )
    target_link_libraries(HummingbirdApp
            ${MYSQL_LIBRARIES}
    )
    include_directories(
            ${MYSQL_INCLUDE_DIR}
    )
endif()

####kasper specific#####################################################
option(WITHHUMMINGBIRDKASPERSPECIFIC "WITH HummingBirdKasperSpecific" OFF)
if (WITHHUMMINGBIRDKASPERSPECIFIC)
    message("Building with Kasperspecific")
    set(HUMMINGBIRDKASPERSPECIFIC_DIR ${HUMMINGBIRD_DIR}/HummingBirdKasperSpecific)
    set(HUMMINGBIRDKASPERSPECIFICFILES
            ../HummingBirdKasperSpecific/src/HelloLibrary.cpp
            ../HummingBirdKasperSpecific/src/HelloLibrary.h
            ../HummingBirdKasperSpecific/src/Hellebrekers/VisualLinkLauncher.cpp
            ../HummingBirdKasperSpecific/src/Hellebrekers/VisualLinkLauncher.h
            HummingBirdCore/src/UIWindows/Widget/DataViewer.h
    )

    add_library(
            KasperSpecificTools STATIC
            ${HUMMINGBIRDKASPERSPECIFICFILES}
    )

    #set cpp def
    add_definitions(-DWITHHUMMINGBIRDKASPERSPECIFIC)

    #link
    target_link_libraries(HummingbirdCore
            KasperSpecificTools
    )
    target_link_libraries(HummingbirdApp
            KasperSpecificTools
    )

    #include
    include_directories(
            ../HummingBirdKasperSpecific/src
    )
endif ()

########################################################################
# Base Link libraries
#TODO: MAKE THIS MAC SPECIFIC
target_link_libraries(HummingbirdCore
        ${SECURITY_FRAMEWORK}
)

target_link_libraries(HummingbirdApp
        ${SECURITY_FRAMEWORK}
)

# get IOKit
find_library(IOKit IOKit)
if(NOT IOKit)
    message(FATAL_ERROR "IOKit not found")
else()
    message("IOKit found")
    target_link_libraries(HummingbirdCore
            ${IOKit}
    )
    target_link_libraries(HummingbirdApp
            ${IOKit}
    )
endif()

# get CoreFoundation
find_library(CoreFoundation CoreFoundation)
if(NOT CoreFoundation)
    message(FATAL_ERROR "CoreFoundation not found")
else()
    message("CoreFoundation found")
    target_link_libraries(HummingbirdCore
            ${CoreFoundation}
    )
    target_link_libraries(HummingbirdApp
            ${CoreFoundation}
    )
endif()


# Find the System Configuration framework
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if(NOT SYSTEM_CONFIGURATION_FRAMEWORK)
    message(FATAL_ERROR "SystemConfiguration not found")
else()
    message("SystemConfiguration found")
    target_link_libraries(HummingbirdCore
            ${SYSTEM_CONFIGURATION_FRAMEWORK}
    )
    target_link_libraries(HummingbirdApp
            ${SYSTEM_CONFIGURATION_FRAMEWORK}
    )
endif()


########################################################################
# Copying resources from source to build
message("Copying resources from ${CMAKE_CURRENT_SOURCE_DIR}/Assets to ${CMAKE_CURRENT_BINARY_DIR}/Assets")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})