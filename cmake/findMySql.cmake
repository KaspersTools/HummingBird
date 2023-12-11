# FindMySQL.cmake

# Try to find MySQL on the system.

# This module will set the following variables:
#   MySQL_FOUND - True if MySQL was found.
#   MySQL_INCLUDE_DIRS - Include directories for MySQL.
#   MySQL_LIBRARIES - The libraries needed to use MySQL.
#   MySQL_VERSION_STRING - The version of MySQL found.

# Check for Homebrew and set hints accordingly
find_program(HOMEBREW_EXECUTABLE brew)
if(HOMEBREW_EXECUTABLE)
    execute_process(
            COMMAND ${HOMEBREW_EXECUTABLE} --prefix
            OUTPUT_VARIABLE HOMEBREW_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Homebrew MySQL include and lib directories
    set(HOMEBREW_MYSQL_INCLUDE_DIR "${HOMEBREW_PREFIX}/include/mysql")
    set(HOMEBREW_MYSQL_LIB_DIR "${HOMEBREW_PREFIX}/lib")
else()
    set(HOMEBREW_MYSQL_INCLUDE_DIR "")
    set(HOMEBREW_MYSQL_LIB_DIR "")
endif()

find_path(MySQL_INCLUDE_DIR
        NAMES mysql.h
        PATHS ${HOMEBREW_MYSQL_INCLUDE_DIR} /usr/include/mysql /usr/local/include/mysql /usr/mysql/include/mysql
        C:/Program Files/MySQL/*/include
)

find_library(MySQL_LIBRARY
        NAMES mysqlclient mysql libmysql
        PATHS ${HOMEBREW_MYSQL_LIB_DIR} /usr/lib /usr/local/lib /usr/mysql/lib/mysql
        C:/Program Files/MySQL/*/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL DEFAULT_MSG
        MySQL_LIBRARY
        MySQL_INCLUDE_DIR
)

if(MySQL_FOUND)
    set(MySQL_INCLUDE_DIRS ${MySQL_INCLUDE_DIR})
    set(MySQL_LIBRARIES ${MySQL_LIBRARY})
endif()

mark_as_advanced(MySQL_INCLUDE_DIR MySQL_LIBRARY)
