ADD_EXECUTABLE( pretty-executable
  pretty-executable.cpp )
TARGET_LINK_LIBRARIES( pretty-executable
  ITKCommon
  ITKIO
  )
ADD_CUSTOM_COMMAND( TARGET pretty-executable
  POST_BUILD
  COMMAND cmake -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/pretty-executable-gdb.py
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
  )


CONFIGURE_FILE( gdb_test_script.gdb.in
  ${CMAKE_CURRENT_BINARY_DIR}/gdb_test_script.gdb
  @ONLY )
ADD_TEST( gdbprettytest
  gdb
  --batch
  -x ${CMAKE_CURRENT_BINARY_DIR}/gdb_test_script.gdb
  )
