#Define sources
SET( vtkRenderingAddOn_SRCS2
  vtkImage3DCroppingBoxCallback.cxx
  vtkImage3DImagePlaneCallback.cxx
  vtkImageBlendWithMask.cxx
  vtkInteractorStyleImage2D.cxx
  vtkInteractorStyleImage3D.cxx
  vtkLookupTableManager.cxx
  vtkOrientationAnnotation.cxx
  vtkOrientedBoxWidget.cxx
  vtkViewImage.cxx
  vtkViewImage2D.cxx
  vtkViewImage2DCollection.cxx
  vtkViewImage2DCollectionCommand.cxx
  vtkViewImage2DCommand.cxx
  vtkViewImage3D.cxx
  vtkViewImage3DCommand.cxx
  )

INCLUDE_DIRECTORIES( BEFORE
  ${MEGAVTK2_BINARY_DIR}
  ${MEGAVTK2_BINARY_DIR}/vtkRenderingAddOn
)

ADD_LIBRARY( vtkRenderingAddOn2
  ${vtkRenderingAddOn_SRCS2}
  )

SET(vtkRenderingAddOn_LIBS2
  vtkCommon
  vtkRendering
  vtkWidgets
  vtkVolumeRendering
  vtkHybrid
  vtkImaging
  )

TARGET_LINK_LIBRARIES( vtkRenderingAddOn2
  ${vtkRenderingAddOn_LIBS2}
  )

SET_TARGET_PROPERTIES( vtkRenderingAddOn2 PROPERTIES VERSION ${GOFIGURE2_LIB_VERSION} SOVERSION ${GOFIGURE2_LIB_VERSION} )

# Runtime
INSTALL( TARGETS vtkRenderingAddOn2
  EXPORT GoFigure2Targets
  RUNTIME DESTINATION ${GOFIGURE2_INSTALL_BIN_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries
  LIBRARY DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} NAMELINK_SKIP COMPONENT Libraries
  )
  
# Development
INSTALL( TARGETS vtkRenderingAddOn2
  EXPORT GoFigure2Targets
  RUNTIME DESTINATION ${GOFIGURE2_INSTALL_BIN_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries
  LIBRARY DESTINATION ${GOFIGURE2_INSTALL_LIB_DIR} COMPONENT Libraries NAMELINK_ONLY
  )
  

