#NEW LIST |
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../cmake/)

list(APPEND HUMMINGBIRD_LIBRARIES
        HummingBirdCore
)

#
## imgui
add_subdirectory(imgui)
#enable IMGUI_ENABLE_STDLIB_SUPPORT
set(IMGUI_ENABLE_STDLIB_SUPPORT ON CACHE BOOL "" FORCE)
set(IMGUI_EXAMPLES ON CACHE BOOL "" FORCE)
set(IMGUI_DEMO ON CACHE BOOL "" FORCE)
set(IMGUI_ENABLE_FREETYPE ON CACHE BOOL "" FORCE)
set(IMGUI_IMPL_GLFW ON CACHE BOOL "" FORCE)
set(IMGUI_IMPL_VULKAN ON CACHE BOOL "" FORCE)

target_link_libraries(HummingBirdCore PUBLIC imgui imgui-glfw imgui-vulkan imgui-stdlib)
list(APPEND HUMMINGBIRD_LIBRARIES
        imgui
        imgui-glfw
        imgui-vulkan
        imgui-stdlib
)

#fmt
IF (NOT EXISTS ../vendor/fmt)
  message(FATAL_ERROR "fmt folder not found")
ELSE ()
  message(STATUS "fmt folder found")
  add_subdirectory(../vendor/fmt)
  target_link_libraries(HummingBirdCore PUBLIC fmt::fmt-header-only)

  target_compile_definitions(HummingBirdCore PUBLIC FMT_HEADER_ONLY)
  list(APPEND HUMMINGBIRD_LIBRARIES
          fmt::fmt-header-only
  )
ENDIF ()

#spdlog
IF (NOT EXISTS ../vendor/spdlog)
  message(FATAL_ERROR "spdlog folder not found")
ELSE ()
  message(STATUS "spdlog folder found")
  add_subdirectory(../vendor/spdlog)
  target_link_libraries(HummingBirdCore PUBLIC spdlog::spdlog)
  target_compile_definitions(HummingBirdCore PUBLIC SPDLOG_FMT_EXTERNAL)
  list(APPEND HUMMINGBIRD_LIBRARIES
          spdlog::spdlog
  )
ENDIF ()

#portable-file-dialogs
IF (NOT EXISTS ../vendor/portable-file-dialogs)
  message(FATAL_ERROR "portable-file-dialogs folder not found")
ELSE ()
  message(STATUS "portable-file-dialogs folder found")
  add_subdirectory(../vendor/portable-file-dialogs)
  target_link_libraries(HummingBirdCore PUBLIC portable_file_dialogs)
  list(APPEND HUMMINGBIRD_LIBRARIES
          portable_file_dialogs
  )
endif ()

##################system##################
# get Security Framework mac
find_library(SECURITY_FRAMEWORK Security)
if (NOT SECURITY_FRAMEWORK)
  message(FATAL_ERROR "Security not found Are you on mac?")
else ()
  message(STATUS "Security found")
  target_link_libraries(HummingBirdCore PUBLIC
          ${SECURITY_FRAMEWORK}
  )

  list(APPEND HUMMINGBIRD_LIBRARIES
          ${SECURITY_FRAMEWORK}
  )
endif ()

# get IOKit mac
find_library(IOKit IOKit)
if (NOT IOKit)
  message(FATAL_ERROR "IOKit not found Are you on mac?")
else ()
  message(STATUS "IOKit found")
  target_link_libraries(HummingBirdCore PUBLIC
          ${IOKit}
  )

  list(APPEND HUMMINGBIRD_LIBRARIES
          ${IOKit}
  )
endif ()


# get CoreFoundation mac
find_library(CoreFoundation CoreFoundation)
if (NOT CoreFoundation)
  message(FATAL_ERROR "CoreFoundation not found Are you on mac?")
else ()
  message(STATUS "CoreFoundation found")
  target_link_libraries(HummingBirdCore PUBLIC
          ${CoreFoundation}
  )

  list(APPEND HUMMINGBIRD_LIBRARIES
          ${CoreFoundation}
  )
endif ()


# Find the System Configuration framework on the system
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if (NOT SYSTEM_CONFIGURATION_FRAMEWORK)
  message(FATAL_ERROR "SystemConfiguration not found Are you on mac?")
else ()
  message(STATUS "SystemConfiguration found")

  target_link_libraries(HummingBirdCore PUBLIC
          ${SYSTEM_CONFIGURATION_FRAMEWORK})

  list(APPEND HUMMINGBIRD_LIBRARIES ${SYSTEM_CONFIGURATION_FRAMEWORK})
endif ()


#libxml2
find_package(LibXml2)
if (NOT LIBXML2_FOUND)
  message(FATAL_ERROR "libxml2 not found")
else ()
  message(STATUS "libxml2 found")
  target_link_libraries(HummingBirdCore PUBLIC ${LIBXML2_LIBRARIES})
  target_include_directories(HummingBirdCore PRIVATE ${LIBXML2_INCLUDE_DIR})

  list(APPEND HUMMINGBIRD_LIBRARIES
          ${LIBXML2_LIBRARIES}
  )
endif ()

########HummingBirdTools########
#sql tool
option(HUMMINGBIRD_WITH_SQL "HummingBird SQL" ON)

if(HUMMINGBIRD_WITH_SQL)
  set(HUMMINGBIRD_VENDOR_SqlToolDir ${HUMMINGBIRD_DIR}/vendor/HummingBird/SqlTool CACHE STRING "HummingBird SqlTool directory")
  add_subdirectory(${HUMMINGBIRD_VENDOR_SqlToolDir})
  target_link_libraries(HummingBirdCore PUBLIC HummingBird_Sql)
  target_compile_definitions(HummingBirdCore PUBLIC HUMMINGBIRD_WITH_SQL)
endif()


#imgui filedialog
#add_subdirectory(ImGuiFileDialog)
#target_include_directories(ImGuiFileDialog PUBLIC
#        ${HUMMINGBIRD_DIR}/vendor/imgui
#        ${HUMMINGBIRD_DIR}/vendor/imgui/backends
#        ${HUMMINGBIRD_DIR}/vendor/imgui/addons
#        ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp
#)
#
#target_link_libraries(HummingBirdCore PUBLIC ImGuiFileDialog)
#



#link all to kasper
#target_link_libraries(HummingBirdKasper PUBLIC ${HUMMINGBIRD_LIBRARIES})