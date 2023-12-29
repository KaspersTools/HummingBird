w#cmake_minimum_required(VERSION 3.12)
#project(HUMMINGBIRD_CORE)
#
#set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
## Add ImGui source
#set(IMGUI_SOURCES
#        ../vendor/imgui/imgui.cpp
#        ../vendor/imgui/imgui_demo.cpp
#        ../vendor/imgui/imgui_draw.cpp
#        ../vendor/imgui/imgui_tables.cpp
#        ../vendor/imgui/imgui_widgets.cpp
#        ../vendor/imgui/backends/imgui_impl_opengl3.cpp
#        ../vendor/imgui/backends/imgui_impl_sdl2.cpp
#        ../vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.cpp
#        ../vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.cpp
#)
#set(IMGUI_HEADERS
#        ../vendor/imgui/imgui.h
#        ../vendor/imgui/imconfig.h
#        ../vendor/imgui/imgui_internal.h
#        ../vendor/imgui/imstb_rectpack.h
#        ../vendor/imgui/imstb_textedit.h
#        ../vendor/imgui/imstb_truetype.h
#        ../vendor/imgui/backends/imgui_impl_opengl3.h
#        ../vendor/imgui/backends/imgui_impl_sdl2.h
#        ../vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.h
#        ../vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.h)
#
####################################
########### Main Project ###########
####################################
#set(SOURCES
#        src/main.cpp
#
#        src/Application.cpp
#        src/Application.h
#
#        src/Log.cpp
#        src/Log.h
#
#        src/Terminal/TerminalWindow.cpp
#        src/Terminal/TerminalWindow.h
#
#        src/UIWindows/Themes/Themes.h
#        src/UIWindows/Themes/ThemeManager.cpp
#        src/UIWindows/Themes/ThemeManager.h
#
#        src/stb_image.h
#
#        src/Rendering/Texture.cpp
#        src/Rendering/Texture.h
#
#        src/Security/LogInWindow.cpp
#        src/Security/LogInWindow.h
#        src/Security/LoginManager.cpp
#        src/Security/LoginManager.h
#
#        src/Sql/SqlConnection.cpp
#        src/Sql/SqlConnection.h
#        src/Sql/SqlExample.h
#        src/Sql/SqlManager.cpp
#        src/Sql/SqlManager.h
#
#        src/GeneralTools/FileDialog.h
#        src/GeneralTools/TextEditting/TextEditorWindow.h
#        src/GeneralTools/TextEditting/TextEditorWindow.cpp
#
#        src/System/Hosts/EditHostsWindow.h
#        src/System/Hosts/EditHostsWindow.cpp
#        src/main.cpp
#)
#
#
##create executable
##add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
##create lib
##add_library(${PROJECT_NAME} STATIC ${SOURCES} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
#add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
####################################
########### Linking ################
####################################
#
##logging
##fmt library linked from ../vendor/fmt
#target_link_libraries(${PROJECT_NAME} fmt)
##spdlog library linked from ../vendor/spdlog
#target_link_libraries(${PROJECT_NAME} spdlog)
##glad
#target_link_libraries(${PROJECT_NAME} glad)
##sdl2
#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
##security
#
#
####################################
########### Copy Assets ############
####################################
#file(COPY Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/bin)
#file(COPY Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/lib)
#
####################################
########### Copy Headers ###########
####################################
## Find all header files in the src directory
##imgui
#include_directories(../vendor/imgui)
#
##sdl2
#target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/sdl/include)
##sources
#target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCES})
##imgui
#target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_HEADERS})
##glad
#target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/glad/include)
##spdlog
#target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/spdlog/include)
##fmt
#target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/fmt/include)

#cmake_minimum_required(VERSION 3.12)
#project(HUMMINGBIRD_CORE)
#
#set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
#
####################################
########### Modules ################
####################################
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#
####################################
########### Dependencies ###########
####################################
##security
#find_library(SECURITY_FRAMEWORK Security)
#
#######submodules##########
##fmt

#
##sdl2
#add_subdirectory(vendor/sdl)
#set ( SDL2_INCLUDE_DIR vendor/sdl/include )
#set ( SDL2_LIBRARY SDL2 )
#target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
#
##spdlog
#add_subdirectory(vendor/spdlog)
#target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
#
##glad
#add_library(glad STATIC vendor/glad/src/glad.c)
#target_include_directories(glad PUBLIC vendor/glad/include)
#
## Add ImGui
#set(IMGUI_SOURCES
#        ../vendor/imgui/imconfig.h
#        ../vendor/imgui/imgui.cpp
#        ../vendor/imgui/imgui.h
#        ../vendor/imgui/imgui_demo.cpp
#        ../vendor/imgui/imgui_draw.cpp
#        ../vendor/imgui/imgui_internal.h
#        ../vendor/imgui/imgui_tables.cpp
#        ../vendor/imgui/imgui_widgets.cpp
#        ../vendor/imgui/imstb_rectpack.h
#        ../vendor/imgui/imstb_textedit.h
#        ../vendor/imgui/imstb_truetype.h
#        ../vendor/imgui/backends/imgui_impl_opengl3.cpp
#        ../vendor/imgui/backends/imgui_impl_opengl3.h
#        ../vendor/imgui/backends/imgui_impl_sdl2.cpp
#        ../vendor/imgui/backends/imgui_impl_sdl2.h
#        ../vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.h
#        ../vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.cpp
#        ../vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.h
#        ../vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.cpp
#)
#include_directories(vendor/imgui)
#
####################################
########### Main Project ###########
####################################
#set(SOURCES
#        #        src/main.cpp
#
#        src/Application.cpp
#        src/Application.h
#        src/Log.cpp
#        src/Log.h
#        src/Terminal/TerminalWindow.cpp
#        src/Terminal/TerminalWindow.h
#        src/UIWindows/Themes/Themes.h
#        src/UIWindows/Themes/ThemeManager.cpp
#        src/UIWindows/Themes/ThemeManager.h
#        src/stb_image.h
#        src/Rendering/Texture.cpp
#        src/Rendering/Texture.h
#        src/Security/LogInWindow.cpp
#        src/Security/LogInWindow.h
#        src/Security/LoginManager.cpp
#        src/Security/LoginManager.h
#        src/Sql/SqlConnection.cpp
#        src/Sql/SqlConnection.h
#        src/Sql/SqlExample.h
#        src/Sql/SqlManager.cpp
#        src/Sql/SqlManager.h
#        src/GeneralTools/FileDialog.h
#        src/GeneralTools/TextEditting/TextEditorWindow.h
#        src/GeneralTools/TextEditting/TextEditorWindow.cpp
#        src/System/Hosts/EditHostsWindow.h
#        src/System/Hosts/EditHostsWindow.cpp
#)
#
##create executable
##add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
##create lib
##add_library(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
##create executable
#add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
#
####################################
########### Linking ################
####################################
#
##glad
#target_link_libraries(${PROJECT_NAME} glad)
##security
#


cmake_minimum_required(VERSION 3.12)
project(KBTools)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

###################################
########## Modules ################
###################################
#MYSQL

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###################################
###### 3rd party libraries ########
###################################
######preinstalled########
#mysql
# Add the mysqlclient submodule
add_subdirectory(vendor/mysqlclient)
# Include directories for mysqlclient
include_directories(vendor/mysqlclient/include)

######submodules##########
#sdl2
add_subdirectory(vendor/sdl)
set ( SDL2_INCLUDE_DIR vendor/sdl/include )
set ( SDL2_LIBRARY SDL2 )

#fmt
add_subdirectory(vendor/fmt)

#spdlog
add_subdirectory(vendor/spdlog)

#glad
# Add GLAD source
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Add ImGui source
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
        vendor/imgui/backends/imgui_impl_sdl2.h

        vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.h
        vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.cpp

        vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.h
        vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.cpp

)

###################################
########## Main Project ###########
###################################
set(SOURCES
        src/Application.cpp
        src/Application.h

        src/Log.cpp
        src/Log.h

        src/Terminal/TerminalWindow.cpp
        src/Terminal/TerminalWindow.h

        src/UIWindows/Themes/Themes.h
        src/UIWindows/Themes/ThemeManager.cpp
        src/UIWindows/Themes/ThemeManager.h

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

        src/GeneralTools/FileDialog.h
        src/GeneralTools/TextEditting/TextEditorWindow.h
        src/GeneralTools/TextEditting/TextEditorWindow.cpp

        src/System/Hosts/EditHostsWindow.h
        src/System/Hosts/EditHostsWindow.cpp
)

#create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

###################################
########## Linking ################
###################################
#logging



###################################
########## INCLUDE ################
###################################
#imgui
#include_directories(vendor/imgui)
#target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})

###################################
########## Settings ###############
###################################
#set_target_properties(
#        ${PROJECT_NAME} PROPERTIES
#        CXX_STANDARD 23
#        CXX_STANDARD_REQUIRED ON
#        CXX_EXTENSIONS OFF)


###################################
########## Copy Assets ############
###################################
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/build/bin)