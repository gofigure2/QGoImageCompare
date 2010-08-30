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

