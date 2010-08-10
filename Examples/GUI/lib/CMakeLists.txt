SET( QGOGUILIB_EXAMPLE_SRC
  qgoimageview2d
  qgoimageview3d
  QGoImageInfoTest
  qgolutdialog
  qgotabimageview2d
  qgotabimageview3d

  qgosynchronizedview2dtest
  qgosynchronizedview3dtest
  qgosynchronizedviewmanagertest
  qgosynchronizedviewguitest
  qgocompareexample
)

#---------------------------------------------------------------------------
#---------------------------------------------------------------------------

#ADD QGoVideoRecorderTest if ENABLE_VIDEO_RECORD IS ON
IF( ENABLE_VIDEO_RECORD_FFMPEG )
  SET(QGOGUILIB_EXAMPLE_SRC
    ${QGOGUILIB_EXAMPLE_SRC}
    QGoVideoRecorderTest
    vtkFFMPEGRenderWindowRecorderTest
    )
ENDIF( ENABLE_VIDEO_RECORD_FFMPEG )

#ADD QGoVideoRecorderTest if ENABLE_VIDEO_RECORD IS ON
IF( ENABLE_VIDEO_RECORD_AVI )
  SET(QGOGUILIB_EXAMPLE_SRC
    ${QGOGUILIB_EXAMPLE_SRC}
    QGoVideoRecorderTest
    vtkAVIRenderWindowRecorderTest
    )
ENDIF( ENABLE_VIDEO_RECORD_AVI )

#---------------------------------------------------------------------------
#---------------------------------------------------------------------------

FOREACH( var ${QGOGUILIB_EXAMPLE_SRC} )

  ADD_EXECUTABLE( ${var} ${var} )
  TARGET_LINK_LIBRARIES( ${var}
    QGoGui
  )

ENDFOREACH( var ${QGOGUILIB_EXAMPLE_SRC} )

ADD_TEST( qgoimageview2dTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgoimageview2d
  ${TESTING_DATA_PATH}/Circle.png
  1
)

ADD_TEST( qgoimageview3dTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgoimageview3d
  ${TESTING_DATA_PATH}/Circle3D.mhd
  1
)

ADD_TEST( QGoImageInfoTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoImageInfoTest
  1
)

ADD_TEST( qgolutdialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgolutdialog
  1
)

ADD_TEST( qgotabelementbaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabelementbase
)

ADD_TEST( qgotabimageviewelementbaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabimageviewelementbase
)

ADD_TEST( qgotabimageview2dTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabimageview2d
  ${TESTING_DATA_PATH}/Circle.png
  1
)

ADD_TEST( qgotabimageview3dTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabimageview3d
  ${TESTING_DATA_PATH}/Circle3D.mhd
  1
)

ADD_TEST( qgotabimageview3dwtTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabimageview3dwt
  ${TESTING_DATA_PATH}/earpax2isl11.lsm
  1
)

ADD_TEST( qgomanualsegmentationsettingsdialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgomanualsegmentationsettingsdialog
  1
)

ADD_TEST( qgonavigationdockwidgetTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgonavigationdockwidget
  1
)

ADD_TEST( qgomanualsegmentationdockwidgetTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgomanualsegmentationdockwidget
  1
)

ADD_TEST( contourmeshstructurehelperTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/contourmeshstructurehelpertest
)

ADD_TEST( contourmeshstructurehelperTest2
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/contourmeshstructurehelpertest2
)

ADD_TEST( QGoLsmToMegaExportDialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoLsmToMegaExportDialogTest
  1
)

ADD_TEST( QGoLsmToMegaExportThreadTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoLsmToMegaExportThreadTest
  earpax2isl11.lsm
  ${TESTING_DATA_PATH}/earpax2isl11.lsm
  ${GOFIGURE2_BINARY_DIR}/Testing/earpax2isl11/
  1
)

ADD_TEST( QGoWizardDBTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoWizardDBTest
  1
)

ADD_TEST( QGoPrintDatabaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoPrintDatabaseTest
  1
)

ADD_TEST( QGoDBCreateBookmarkDialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoDBCreateBookmarkDialogTest
  1
)

ADD_TEST( QGoDeleteDBEntityDialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoDeleteDBEntityDialog
  1
)

ADD_TEST( qgotabimageview3dwtTest2
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgotabimageview3dwt2
  ${GOFIGURE2_BINARY_DIR}/Testing/earpax2isl11/earpax2isl11.lsm-PL00-CO00-RO00-ZT00-YT00-XT00-TM0000-ch00-zs0000.png
)

SET_TESTS_PROPERTIES( qgotabimageview3dwtTest2
  PROPERTIES DEPENDS
  lsmtomegacaptureTest
)


ADD_TEST( qgosynchronizedview2dtest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgosynchronizedview2dtest
  ${TESTING_DATA_PATH}/Circle.png
  1
)

ADD_TEST( qgosynchronizedview3dtest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgosynchronizedview3dtest
  ${TESTING_DATA_PATH}/Circle3D.mhd
  1
)

ADD_TEST( qgosynchronizedviewmanagertest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgosynchronizedviewmanagertest
  ${TESTING_DATA_PATH}/Circle.png
  ${TESTING_DATA_PATH}/Circle3D.mhd
  1
)

#ADD_TEST( qgosynchronizedviewguitest
#  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgosynchronizedviewguitest
#  ${TESTING_DATA_PATH}/Circle.png
#  ${TESTING_DATA_PATH}/Circle3D.mhd
#  1
#)

ADD_TEST( QGoLevelset2DTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoLevelset2DTest
  ${TESTING_DATA_PATH}/Circle3D.mhd
  1
)

ADD_TEST( QGoDBInitCreateAuthorPageTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoDBInitCreateAuthorPageTest
  1
)

ADD_TEST( QGoDBInitCreateMicroscopePageTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoDBInitCreateMicroscopePageTest
  1
)

#---------------------------------------------------------------------------
#---------------------------------------------------------------------------

# ADD THIS TEST IF ENABLE_VIDEO_RECORD_FFMPEG IS ON
IF( ENABLE_VIDEO_RECORD_FFMPEG )
  ADD_TEST( QGoVideoRecorderTest
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoVideoRecorderTest
    1
  )

  ADD_TEST( vtkFFMPEGRenderWindowRecorderTest
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vtkFFMPEGRenderWindowRecorderTest
    test.avi
  )
ENDIF( ENABLE_VIDEO_RECORD_FFMPEG )

# ADD THIS TEST IF ENABLE_VIDEO_RECORD_AVI IS ON
IF( ENABLE_VIDEO_RECORD_AVI )
  ADD_TEST( QGoVideoRecorderTest
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoVideoRecorderTest
    1
  )

  ADD_TEST( vtkAVIRenderWindowRecorderTest
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vtkAVIRenderWindowRecorderTest
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.avi
  )
ENDIF( ENABLE_VIDEO_RECORD_AVI )

#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
