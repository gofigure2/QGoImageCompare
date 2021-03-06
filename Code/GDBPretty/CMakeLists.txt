FIND_PACKAGE(PythonLibs REQUIRED)

INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )

SET( QT_USE_QTNETWORK TRUE )
INCLUDE( ${QT_USE_FILE} )

SET( ICPGUI_SRCS_CXX
  icpGuiMain.cxx
  QImageReceiver.cxx
  )
SET( ICPGUI_MOC_HDRS
  QImageReceiver.h
  )

QT4_WRAP_CPP( ICPGUI_MOC_CXX ${ICPGUI_MOC_HDRS} )

ADD_EXECUTABLE( icpGui
  ${ICPGUI_SRCS_CXX}
  ${ICPGUI_MOC_CXX}
  )
TARGET_LINK_LIBRARIES( icpGui QGoGui )
SET_TARGET_PROPERTIES( icpGui
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
  )
GET_TARGET_PROPERTY( ICPGUI_BUILD_LOCATION icpGui RUNTIME_OUTPUT_DIRECTORY )
# @TODO figure out where we want to install it
SET( ICPGUI_INSTALL_LOCATION
  "${CMAKE_INSTALL_PREFIX}/share/gdb/python/"
  CACHE PATH "Where to install the gdb icp pretty-printer.  Should be
  somewhere in Python's sys.path." )

CONFIGURE_FILE( icpmodule.cxx.in icpmodule.cxx @ONLY )

SET_SOURCE_FILES_PROPERTIES( icpmodule.cxx PROPERTIES
  OBJECT_DEPENDS "${ICPGUI_MOC_CXX}"
)

PYTHON_ADD_MODULE( icp
  ${CMAKE_CURRENT_BINARY_DIR}/icpmodule.cxx )
# TODO: The following should be done in the upstream cmake module.
SET_TARGET_PROPERTIES( icp PROPERTIES
  PREFIX "" )
TARGET_LINK_LIBRARIES( icp QGoGui ${PYTHON_LIBRARIES})

INSTALL( TARGETS icp icpGui
  RUNTIME DESTINATION ${ICPGUI_INSTALL_LOCATION}
  LIBRARY DESTINATION ${ICPGUI_INSTALL_LOCATION}
  ARCHIVE DESTINATION ${ICPGUI_INSTALL_LOCATION}
  )
