# FindMySQL.cmake

# Try to find MySQL on the system.

# This module will set the following variables:
#   MySQL_FOUND - True if MySQL was found.
#   MySQL_INCLUDE_DIRS - Include directories for MySQL.
#   MySQL_LIBRARIES - The libraries needed to use MySQL.
#   MySQL_VERSION_OK - The version of MySQL found.
set(Minimum_MySQL_Version 8.2)

message(STATUS "----------------- Looking for MySQL -----------------")

# Check for Homebrew and set hints accordingly
find_program(HOMEBREW_EXECUTABLE brew)

if (HOMEBREW_EXECUTABLE)
  message(STATUS "Homebrew found at ${HOMEBREW_EXECUTABLE} - using it to find MySQL")
  execute_process(
          COMMAND ${HOMEBREW_EXECUTABLE} --prefix
          OUTPUT_VARIABLE HOMEBREW_PREFIX
          OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  # Homebrew MySQL include and lib directories
  set(HOMEBREW_MYSQL_INCLUDE_DIR "${HOMEBREW_PREFIX}/include/mysql")
  set(HOMEBREW_MYSQL_LIB_DIR "${HOMEBREW_PREFIX}/lib")
  set(HOMEBREW_MYSQL_BIN_DIR "${HOMEBREW_PREFIX}/bin")
else ()
  set(HOMEBREW_MYSQL_INCLUDE_DIR "")
  set(HOMEBREW_MYSQL_LIB_DIR "")
endif ()

# Find the MacPorts executable
find_program(MACPORTS_EXECUTABLE port)
if (MACPORTS_EXECUTABLE)
  message(STATUS "MacPorts found at ${MACPORTS_EXECUTABLE} - using it to find MySQL")

  get_filename_component(MACPORTS_BIN_DIR ${MACPORTS_EXECUTABLE} DIRECTORY)
  get_filename_component(MACPORTS_PREFIX ${MACPORTS_BIN_DIR} DIRECTORY)


  message(STATUS "MACPORTS_PREFIX: ${MACPORTS_PREFIX}")

  #find any folder in /include that starts with mysql
  file(GLOB MACPORTS_MYSQL_INCLUDE_DIR "${MACPORTS_PREFIX}/include/mysql*/mysql")
  file(GLOB MACPORTS_MYSQL_LIB_DIR "${MACPORTS_PREFIX}/lib/mysql*/mysql")
  file(GLOB MACPORTS_MYSQL_BIN_DIR "${MACPORTS_PREFIX}/lib/mysql*/bin")

  message(STATUS "MACPORTS_MYSQL_INCLUDE_DIR: ${MACPORTS_MYSQL_INCLUDE_DIR}")
  message(STATUS "MACPORTS_MYSQL_LIB_DIR: ${MACPORTS_MYSQL_LIB_DIR}")
  message(STATUS "MACPORTS_MYSQL_BIN_DIR: ${MACPORTS_MYSQL_BIN_DIR}")
else ()
  set(MACPORTS_MYSQL_INCLUDE_DIR "")
  set(MACPORTS_MYSQL_LIB_DIR "")
endif ()

find_path(MySQL_INCLUDE_DIR
        NAMES mysql.h PATHS
        ${HOMEBREW_MYSQL_INCLUDE_DIR}
        ${MACPORTS_MYSQL_INCLUDE_DIR}
        /usr/local/include/mysql
        /usr/include/mysql
        C:/Program Files/MySQL/*/include
)

find_library(MySQL_LIBRARY
        NAMES mysqlclient mysql libmysql PATHS
        ${HOMEBREW_MYSQL_LIB_DIR}
        ${MACPORTS_MYSQL_LIB_DIR}
        /usr/lib
        /usr/local/lib
        /usr/mysql/lib/mysql
        C:/Program Files/MySQL/*/lib
)


# Define potential bin directories for MySQL
set(MySQL_BIN_DIRS
        ${HOMEBREW_MYSQL_BIN_DIR}
        ${MACPORTS_MYSQL_BIN_DIR}
        /usr/bin
        /usr/local/bin
        /usr/mysql/bin
        C:/Program Files/MySQL/*/bin
)

message(STATUS "MySQL_BIN_DIRS: ${MySQL_BIN_DIRS}")
message(STATUS "MySQL_LIBRARY: ${MySQL_LIBRARY}")
message(STATUS "MySQL_INCLUDE_DIR: ${MySQL_INCLUDE_DIR}")

find_program(MYSQL_CONFIG_EXECUTABLE NAMES mysql_config PATHS ${MySQL_BIN_DIRS}  NO_DEFAULT_PATH)

if (NOT MYSQL_CONFIG_EXECUTABLE)
  #  message(FATAL_ERROR "mysql_config not found")
  set(MySQL_VERSION_OK FALSE)
  message(FATAL_ERROR "mysql_config not found")
else ()
  execute_process(
          COMMAND ${MYSQL_CONFIG_EXECUTABLE} --version
          OUTPUT_VARIABLE MYSQL_VERSION_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Parse the version number
  string(REGEX MATCH "([0-9]+\\.[0-9]+\\.[0-9]+)" MySQL_VERSION_STRING ${MYSQL_VERSION_OUTPUT})

  # Compare with the minimum required version
  if (MySQL_VERSION_STRING VERSION_LESS Minimum_MySQL_Version)
    set(MySQL_VERSION_OK FALSE)
    message(WARNING "MySQL version ${MySQL_VERSION_STRING} found, but version ${Minimum_MySQL_Version} or greater is required.")
  else ()
    set(MySQL_VERSION_OK TRUE)
    message(STATUS "MySQL version ${MySQL_VERSION_STRING} found")
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL DEFAULT_MSG
        MySQL_LIBRARY
        MySQL_INCLUDE_DIR
        MySQL_VERSION_OK
)

if (MySQL_FOUND)
  message(STATUS "MySQL found, enabling MySQL support")
  set(MySQL_INCLUDE_DIRS ${MySQL_INCLUDE_DIR})
  set(MySQL_LIBRARIES ${MySQL_LIBRARY})
  set(MySQL_VERSION_STRING ${MySQL_VERSION_STRING})
  set(MySQL_VERSION_OK ${MySQL_VERSION_OK})
endif()

mark_as_advanced(MySQL_INCLUDE_DIRS MySQL_LIBRARIES MySQL_VERSION_STRING MySQL_VERSION_OK)