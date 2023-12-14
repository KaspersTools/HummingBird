cmake_minimum_required(VERSION 3.12)
project(HUMMINGBIRD_FRONT)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)

# Add ImGui source
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/backends/imgui_impl_sdl2.cpp
        vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.cpp
        vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.cpp
)

set(IMGUI_HEADERS
        vendor/imgui/imgui.h
        vendor/imgui/imconfig.h
        vendor/imgui/imgui_internal.h
        vendor/imgui/imstb_rectpack.h
        vendor/imgui/imstb_textedit.h
        vendor/imgui/imstb_truetype.h
        vendor/imgui/backends/imgui_impl_opengl3.h
        vendor/imgui/backends/imgui_impl_sdl2.h
        vendor/imgui/vendor/filedialog/FileBrowser/ImGuiFileBrowser.h
        vendor/imgui/vendor/ImGuiColorTextEdit/TextEditor.h)

###################################
########## Main Project ###########
###################################
set(SOURCES
    main.cpp
)


#create executable
#add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
#create lib
#add_library(${PROJECT_NAME} STATIC ${SOURCES} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
#create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
###################################
########## Linking ################
###################################

#logging
#fmt library linked from vendor/fmt
target_link_libraries(${PROJECT_NAME} fmt:fmt)
#spdlog library linked from vendor/spdlog
target_link_libraries(${PROJECT_NAME} spdlog)
#glad
target_link_libraries(${PROJECT_NAME} glad)
#sdl2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
#security
target_link_libraries(${PROJECT_NAME} ${SECURITY_FRAMEWORK})
#hummingbirdcore
#target_link_libraries(${PROJECT_NAME} HUMMINGBIRD_CORE)

###################################
########## Copy Assets ############
###################################
#file(COPY Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/bin)
#file(COPY Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/lib)

#imgui
include_directories(vendor/imgui)
###################################
########## Target Include #########
###################################
# Find all header files in the src directory
#sources
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCES})
#sdl2
target_include_directories(${PROJECT_NAME} PUBLIC vendor/sdl/include)
#imgui
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_HEADERS})
#glad
target_include_directories(${PROJECT_NAME} PUBLIC vendor/glad/include)
#spdlog
target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)
#fmt
target_include_directories(${PROJECT_NAME} PUBLIC vendor/fmt/include)
#hummingbirdcore
target_include_directories(${PROJECT_NAME} PUBLIC ../HummingbirdCore/src)