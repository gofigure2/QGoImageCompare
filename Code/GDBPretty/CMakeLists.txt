FIND_PACKAGE(PythonLibs REQUIRED)

INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_DIRS} )

PYTHON_ADD_MODULE( icp
  icp.cpp )
# TODO: The following should be done in the upstream cmake module.
SET_TARGET_PROPERTIES( icp PROPERTIES
  PREFIX "" )
TARGET_LINK_LIBRARIES( icp QGoGui )
