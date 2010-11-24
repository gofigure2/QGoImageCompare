SET( QGOGUILIB_TEST_SRC
  qgoimageview2d
  qgoimageview3d
  qgolutdialog
  qgosynchronizedview2dtest
  qgosynchronizedview3dtest
  qgosynchronizedviewmanagertest
  pipevtktest
  pipeitktest
)


FOREACH( var ${QGOGUILIB_TEST_SRC} )

  ADD_EXECUTABLE( ${var} ${var} )
  TARGET_LINK_LIBRARIES( ${var}
    QGoGui
  )
ENDFOREACH( var ${QGOGUILIB_TEST_SRC} )

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

ADD_TEST( qgolutdialogTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgolutdialog
  1
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

#ADD_TEST( pipeitktest
#  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pipeitktest
#  ${TESTING_DATA_PATH}/Circle3D.mhd
#)

#ADD_TEST( pipevtktest
#  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pipevtktest
#  ${TESTING_DATA_PATH}/Circle3D.mhd
#)

