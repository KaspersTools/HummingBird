cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD)
#TODO: CLEAN UP THIS FUCKING MESS!!!!!!!!!!!!!!!!!!!!!!!!!!
#TODO: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
set(CMAKE_CXX_STANDARD 23)

message(STATUS "------HUMNMING BIRD------")

#DISPLAY ALL INFO HERE ABOUT THE BUILD
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_OUTPUT_DIRECTORY: ${CMAKE_CURRENT_OUTPUT_DIRECTORY}")
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(HUMMINGBIRD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HUMMINGBIRDCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/HummingbirdCore)
set(HUMMINGBIRDFRONT_DIR ${HUMMINGBIRD_DIR}/HummingbirdFront)

# Add cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#options
option(WITHHUMMINGBIRDCOREASLIB "WITH HummingBirdCore as lib" OFF)

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


####MAIN PROJECT####
# HummingbirdCore
set(HUMMINGBIRDCOUREBASESRC
        HummingBirdCore/src/CoreRef.h
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
        HummingBirdCore/src/System/SysInfo.cpp
        HummingBirdCore/src/System/SysInfo.h
        HummingBirdCore/src/System/SysInfoWindow.cpp
        HummingBirdCore/src/System/SysInfoWindow.h
        HummingBirdCore/src/Sql/SqlWindow.cpp
        HummingBirdCore/src/Sql/SqlWindow.h
        HummingBirdCore/src/Sql/SqlConnection.cpp
        HummingBirdCore/src/Sql/SqlConnection.h
        HummingBirdCore/src/UIWindows/Widget/DataViewer.h
        HummingBirdCore/src/ThirdParty/MetricsImGui.cpp
        HummingBirdCore/src/ThirdParty/MetricsImGui.h
        HummingBirdCore/src/UIWindows/Widget/MetricsWidget.h
        HummingBirdCore/src/Utils/UIUtils.h
        HummingBirdCore/src/System/LaunchDaemonsManager.cpp
        HummingBirdCore/src/System/LaunchDaemonsManager.h
        HummingBirdCore/src/Utils/Plist/PlistUtil.h
)

if (NOT WITHHUMMINGBIRDCOREASLIB)
  message("Building with HummingBirdCore as exe")
  add_executable(HummingbirdCoreEXE
          ${HUMMINGBIRDCOUREBASESRC}
          ${HUMMINGBIRDCORE_DIR}/src/main.cpp
  )
  target_precompile_headers(HummingbirdCoreEXE PUBLIC HummingBirdCore/src/PCH/pch.h)
endif ()

add_library(HummingbirdCore STATIC
        ${HUMMINGBIRDCOUREBASESRC}
)
target_precompile_headers(HummingbirdCore PUBLIC HummingBirdCore/src/PCH/pch.h)

# Core include directories
include_directories(
        HummingBirdCore/src
)
########################################################################
#tinyxml2
find_package(LibXml2 REQUIRED)
if (NOT LIBXML2_FOUND)
  message(FATAL_ERROR "libxml2 not found")
else ()
message("libxml2 found")

  #include
  include_directories(
          ${LIBXML2_INCLUDE_DIRS}
  )

  #link
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${LIBXML2_LIBRARIES}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${LIBXML2_LIBRARIES}
  )
endif ()

#mysql
find_package(MySQL)

if (NOT MySQL_FOUND)
  message(WARNING "MySQL not found")
else ()
  message("MySQL found")
  #include
  include_directories(
          ${MySQL_INCLUDE_DIRS}
  )
  #link
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${MySQL_LIBRARIES}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${MySQL_LIBRARIES}
  )
endif ()

######vendor

# fmt
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/fmt)
  message("fmt folder not found")
else ()
  message("fmt folder found")
  add_subdirectory(${HUMMINGBIRD_DIR}/vendor/fmt)

  #include
  include_directories(
          ${HUMMINGBIRD_DIR}/vendor/fmt/include
  )

  #link
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            fmt::fmt
    )
  endif ()

  target_link_libraries(HummingbirdCore
          fmt::fmt
  )
endif ()


#sdl
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/sdl)
  message("SDL folder not found")
else ()
  message("SDL folder found needed by imgui")
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_LIBC ON CACHE BOOL "" FORCE)
  set(SDL_TEST OFF CACHE BOOL "" FORCE)

  add_subdirectory(vendor/sdl sdl)

  #include
  include_directories(
          ${HUMMINGBIRD_DIR}/vendor/sdl/include
  )

  #link
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            SDL2-static
    )
  endif ()

  target_link_libraries(HummingbirdCore
          SDL2-static
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
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            spdlog::spdlog
    )
  endif ()

  target_link_libraries(HummingbirdCore
          spdlog::spdlog
  )

endif ()

#imgui
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/imgui)
  message("Imgui folder not found")
else ()
  set(IMGUI_SOURCES
          #imgui
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/imconfig.h

          #imgui widgets
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_draw.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_internal.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_tables.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_widgets.cpp

          #imstb
          ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_textedit.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_rectpack.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/imstb_textedit.h

          #imgui demos
          ${HUMMINGBIRD_DIR}/vendor/imgui/imgui_demo.cpp

          #backends

          vendor/imgui/backends/imgui_impl_opengl3_loader.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/backends/imgui_impl_sdl2.h


          #misc
          ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
          ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h

          #addons
          ${HUMMINGBIRD_DIR}/vendor/imgui/addons/imguinotify/imgui_notify.h

          #fonts
          ${HUMMINGBIRD_DIR}/vendor/imgui/addons/imguinotify/font_awesome_5.h
          ${HUMMINGBIRD_DIR}/vendor/imgui/addons/imguinotify/fa_solid_900.h
  )

  add_library(imgui STATIC
          ${IMGUI_SOURCES}
  )

  #include imgui
  include_directories(
          vendor/imgui
          vendor/imgui/backends
          vendor/imgui/misc/cpp
          vendor/imgui/addons/imguinotify
  )


  # Link imgui
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            imgui
    )
  endif ()

  target_link_libraries(HummingbirdCore
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
  )

  add_library(
          KasperSpecificTools STATIC
          ${HUMMINGBIRDKASPERSPECIFICFILES}
  )

  #set cpp def
  add_definitions(-DWITHHUMMINGBIRDKASPERSPECIFIC)

  #include
  include_directories(
          ../HummingBirdKasperSpecific/src
  )

  #link
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            KasperSpecificTools
    )
  endif ()

  target_link_libraries(HummingbirdCore
          KasperSpecificTools
  )

endif ()

########################################################################
# Base Link libraries
#TODO: MAKE THIS MAC SPECIFIC

# get Security
find_library(SECURITY_FRAMEWORK Security)
if (NOT SECURITY_FRAMEWORK)
  message(FATAL_ERROR "Security not found Are you on mac?")
else ()
  message("Security found")
  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${SECURITY_FRAMEWORK}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${SECURITY_FRAMEWORK}
  )
endif ()

# get IOKit
find_library(IOKit IOKit)
if (NOT IOKit)
  message(FATAL_ERROR "IOKit not found Are you on mac?")
else ()
  message("IOKit found")

  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${IOKit}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${IOKit}
  )
endif ()

# get CoreFoundation
find_library(CoreFoundation CoreFoundation)
if (NOT CoreFoundation)
  message(FATAL_ERROR "CoreFoundation not found Are you on mac?")
else ()
  message("CoreFoundation found")

  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${CoreFoundation}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${CoreFoundation}
  )
endif ()


# Find the System Configuration framework
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if (NOT SYSTEM_CONFIGURATION_FRAMEWORK)
  message(FATAL_ERROR "SystemConfiguration not found Are you on mac?")
else ()
  message("SystemConfiguration found")

  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${SYSTEM_CONFIGURATION_FRAMEWORK}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${SYSTEM_CONFIGURATION_FRAMEWORK}
  )
endif ()

# Find openg;
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message(FATAL_ERROR "OpenGL not found")
else ()
  message("OpenGL found")

  if (NOT WITHHUMMINGBIRDCOREASLIB)
    target_link_libraries(HummingbirdCoreEXE
            ${OPENGL_LIBRARIES}
    )
  endif ()

  target_link_libraries(HummingbirdCore
          ${OPENGL_LIBRARIES}
  )
endif ()

########################################################################
# Copying resources from source to build
message("Copying resources from ${CMAKE_CURRENT_SOURCE_DIR}/Assets to ${CMAKE_CURRENT_BINARY_DIR}/Assets")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})