IF( WIN32 )
  IF( NOT CYGWIN )
    IF( NOT MINGW )
      IF( BUILD_SHARED_LIBS )
        ADD_DEFINITIONS( -DQGoGUI_EXPORT )
      ENDIF( BUILD_SHARED_LIBS )
    ENDIF( NOT MINGW )
  ENDIF( NOT CYGWIN )
ENDIF( WIN32 )

CONFIGURE_FILE( ${GOFIGURE2_SOURCE_DIR}/Code/GUI/lib/QGoGUILibConfigure.h.in
  ${GOFIGURE2_BINARY_DIR}/Code/GUI/lib/QGoGUILibConfigure.h @ONLY IMMEDIATE )


SET( QGoGUILIB_SRC
  QGoImageView.cxx
  QGoImageView2D.cxx
  QGoImageView3D.cxx
  QGoLUTDialog.cxx
  QSplitterChild.cxx
  SnapshotHelper.cxx
  QGoSynchronizedView.cxx
  QGoSynchronizedView2D.cxx
  QGoSynchronizedView3D.cxx
  QGoSynchronizedView2DCallbacks.cxx
  QGoSynchronizedView3DCallbacks.cxx
  QGoSynchronizedViewManager.cxx
  QGoSynchronizedViewMainWindow.cxx
)

#---------------------------------------------------------------------------

#---------------------------------------------------------------------------

SET( QGoGUILIB_HDRS
  QGoImageView.h
  QGoImageView2D.h
  QGoImageView3D.h
  QGoLUTDialog.h
  QSplitterChild.h

  QGoSynchronizedView.h
  QGoSynchronizedView2D.h
  QGoSynchronizedView3D.h
#   QGoSynchronizedView2DCallbacks.h
  QGoSynchronizedView3DCallbacks.h
  QGoSynchronizedViewManager.h
  QGoSynchronizedViewMainWindow.h
)

#---------------------------------------------------------------------------

#---------------------------------------------------------------------------

SET( QGoGUILIB_UI
  Resources/QNameDescriptionInputDialog.ui

  Resources/QGoSynchronizedView.ui
)

#---------------------------------------------------------------------------
#---------------------------------------------------------------------------

QT4_WRAP_UI( QGoGUILIB_UI_H
  ${QGoGUILIB_UI}
)

QT4_WRAP_CPP( QGoGUILIB_MOC ${QGoGUILIB_HDRS} )

QT4_ADD_RESOURCES( QGoGUI_QRC
  ${QGoResourceFile}
)
SET_SOURCE_FILES_PROPERTIES( ${QGoGUILIB_SRC} PROPERTIES
  OBJECT_DEPENDS "${QGoGUILIB_UI_H}"
  )

SET_SOURCE_FILES_PROPERTIES( ${QGoGUILIB_MOC} PROPERTIES
  OBJECT_DEPENDS "${QGoGUILIB_UI_H}"
  )

ADD_LIBRARY( QGoGui
  ${QGoGUILIB_SRC}
  ${QGoGUILIB_MOC}
  ${QGoGUI_QRC}
)

SET( QGoGUI_LIBS
  QVTK
  vtkRenderingAddOn2
  )

TARGET_LINK_LIBRARIES( QGoGui
  ${QGoGUI_LIBS}
  ITKIO
  ITKCommon
  ITKBasicFilters
  ITKStatistics
  )

SET_TARGET_PROPERTIES( QGoGui PROPERTIES VERSION ${GOFIGURE2_LIB_VERSION} SOVERSION ${GOFIGURE2_LIB_VERSION} )

# Runtime
INSTALL( TARGETS QGoGui
  EXPORT GoFigure2Targets
  RUNTIME DESTINATION ${GOFIGURE2_INSTALL_BIN_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries
  LIBRARY DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} NAMELINK_SKIP COMPONENT Libraries
  )
  
# Development
INSTALL( TARGETS QGoGui
  EXPORT GoFigure2Targets
  RUNTIME DESTINATION ${GOFIGURE2_INSTALL_BIN_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries
  LIBRARY DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries NAMELINK_ONLY
  )
  

