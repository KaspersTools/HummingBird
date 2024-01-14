#NEW LIST |
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../cmake/)

# opengl
find_package(OpenGL REQUIRED)
target_link_libraries(HummingBirdCore ${OPENGL_LIBRARIES})

# glfw
if (NOT EXISTS ${HUMMINGBIRD_DIR}/vendor/glfw)
  message(FATAL_ERROR "glfw folder not found")
else ()
  message(STATUS "glfw folder found")
  #todo: make this cross platform
  find_library(GLFW_LIBRARY NAMES glfw glfw3 libglfw3 PATHS ${HUMMINGBIRD_DIR}/vendor/vendor/glfw/lib-x86_64)

  if (NOT GLFW_LIBRARY)
    message(FATAL_ERROR "glfw library not found")
  else ()
    message(STATUS "glfw library found")
    target_link_directories(HummingBirdCore PRIVATE ${HUMMINGBIRD_DIR}/vendor/glfw/lib-x86_64)
    target_link_libraries(HummingBirdCore ${GLFW_LIBRARY})
    target_include_directories(HummingBirdCore PUBLIC ${HUMMINGBIRD_DIR}/vendor/glfw/include)
  endif ()
endif ()

message(STATUS "glfw library found")
message(STATUS "glfw include dir ${CMAKE_CURRENT_SOURCE_DIR}" )

# imgui
set(KDB_IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(KDB_IMGUI_GLFW ON CACHE BOOL "" FORCE)

set(KDB_IMGUI_GLFWINCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include CACHE STRING "" FORCE)

add_subdirectory(imgui)
target_link_libraries(HummingBirdCore KDB_IMGUI-STATIC)
target_include_directories(HummingBirdCore PUBLIC
        ${HUMMINGBIRD_DIR}/vendor/imgui
        ${HUMMINGBIRD_DIR}/vendor/imgui/backends
        ${HUMMINGBIRD_DIR}/vendor/imgui/addons
        ${HUMMINGBIRD_DIR}/vendor/imgui/misc/cpp
      )


#fmt

IF (NOT EXISTS ../vendor/fmt)
  message(FATAL_ERROR "fmt folder not found")
ELSE ()
  message(STATUS "fmt folder found")
  add_subdirectory(../vendor/fmt)
  target_link_libraries(HummingBirdCore fmt::fmt-header-only)
ENDIF ()

#spdlog
IF (NOT EXISTS ../vendor/spdlog)
  message(FATAL_ERROR "spdlog folder not found")
ELSE ()
  message(STATUS "spdlog folder found")
  add_subdirectory(../vendor/spdlog)
  target_link_libraries(HummingBirdCore spdlog::spdlog)
  target_compile_definitions(HummingBirdCore PRIVATE SPDLOG_FMT_EXTERNAL)
ENDIF ()

##################system##################
#mysql
find_package(MySQL)
if (NOT MYSQL_FOUND)
  message(FATAL_ERROR "MySQL not found")
else ()
  message(STATUS "MySQL found")
  target_link_libraries(HummingBirdCore ${MySQL_LIBRARIES})
  target_include_directories(HummingBirdCore PUBLIC ${MySQL_INCLUDE_DIRS})
endif ()

# get Security Framework mac
find_library(SECURITY_FRAMEWORK Security)
if (NOT SECURITY_FRAMEWORK)
  message(FATAL_ERROR "Security not found Are you on mac?")
else ()
  message(STATUS "Security found")
  target_link_libraries(HummingBirdCore
          ${SECURITY_FRAMEWORK}
  )
endif ()

# get IOKit mac
find_library(IOKit IOKit)
if (NOT IOKit)
  message(FATAL_ERROR "IOKit not found Are you on mac?")
else ()
  message(STATUS "IOKit found")
  target_link_libraries(HummingBirdCore
          ${IOKit}
  )
endif ()


# get CoreFoundation mac
find_library(CoreFoundation CoreFoundation)
if (NOT CoreFoundation)
  message(FATAL_ERROR "CoreFoundation not found Are you on mac?")
else ()
  message(STATUS "CoreFoundation found")
  target_link_libraries(HummingBirdCore
          ${CoreFoundation}
  )
endif ()


# Find the System Configuration framework on the system
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if (NOT SYSTEM_CONFIGURATION_FRAMEWORK)
  message(FATAL_ERROR "SystemConfiguration not found Are you on mac?")
else ()
  message(STATUS "SystemConfiguration found")

  target_link_libraries(HummingBirdCore
          ${SYSTEM_CONFIGURATION_FRAMEWORK})
endif ()


#libxml2
find_package(LibXml2)
if (NOT LIBXML2_FOUND)
  message(FATAL_ERROR "libxml2 not found")
else ()
  message(STATUS "libxml2 found")
  target_link_libraries(HummingBirdCore ${LIBXML2_LIBRARIES})
  target_include_directories(HummingBirdCore PRIVATE ${LIBXML2_INCLUDE_DIR})
endif ()
