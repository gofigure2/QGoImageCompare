# - Find MySQL
# This module finds MySQL libraries.
# 
#
# MYSQL_INCLUDE_DIR - Directories to include to use MySQL
# MYSQL_LIBRARIES   - Files to link against to use MySQL
# MYSQL_EXTRA_LIBRARIES - Additional files to link against to use MySQL
# MYSQL_FOUND, If false, you cannot build anything that requires MySQL.

FIND_PATH( MYSQL_INCLUDE_DIR NAMES mysql.h
  PATHS
  "$ENV{ProgramFiles}/MySQL/*/"
  "$ENV{SystemDrive}/MySQL/*/"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MySQL AB\\*;Location]/"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MySQL AB\\*;Location]/"
  PATH_SUFFIXES "include" "include/mysql" "mysql/include" 
  DOC "Specify the directory containing mysql.h"
)

SET( MYSQL_LIBRARY_NAMES mysql libmysql mysqlclient mysqlclient_r )

FIND_LIBRARY( MYSQL_LIBRARIES 
  NAMES ${MYSQL_LIBRARY_NAMES}
  PATHS
  "$ENV{ProgramFiles}/MySQL/*/"
  "$ENV{SystemDrive}/MySQL/*/"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MySQL AB\\*;Location]/lib/opt"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MySQL AB\\*;Location]/lib/opt"
  PATH_SUFFIXES "lib" "mysql" "mysql/lib" "mysql/lib/mysql" "lib/opt"
  DOC "Specify the mysql library here."
)

# On Windows you typically don't need to include any extra libraries
# to build MYSQL stuff.
IF( NOT WIN32 )
  FIND_PACKAGE( ZLIB )
  SET( MYSQL_EXTRA_LIBRARIES ${ZLIB_LIBRARY} )
ELSE( NOT WIN32 )
  SET( MYSQL_EXTRA_LIBRARIES "" )
ENDIF( NOT WIN32 )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MYSQL DEFAULT_MSG 
  MYSQL_INCLUDE_DIR
  MYSQL_LIBRARIES
  MYSQL_EXTRA_LIBRARIES
)

MARK_AS_ADVANCED( MYSQL_FOUND 
  MYSQL_LIBRARIES 
  MYSQL_EXTRA_LIBRARIES 
  MYSQL_INCLUDE_DIR 
)
