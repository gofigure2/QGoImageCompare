IF( BUILD_GDB_PRETTY )
  IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR
      CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
    ADD_SUBDIRECTORY( GDBPretty )
  ELSE()
    MESSAGE( WARNING "To test the gdb pretty module the project must be built
    with CMAKE_BUILD_TYPE set to 'Debug' or 'RelWithDebInfo'." )
  ENDIF()
ENDIF()
