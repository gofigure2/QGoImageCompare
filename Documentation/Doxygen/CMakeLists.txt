IF( LINK_EXTERNAL_DOC )
  FILE( DOWNLOAD http://www.vtk.org/files/nightly/vtkNightlyDoc.tag
    ${GOFIGURE2_BINARY_DIR}/Documentation/vtkNightlyDoc.tag
  )

  FILE( DOWNLOAD http://www.itk.org/files/nightly/InsightDoxygen.tag
    ${GOFIGURE2_BINARY_DIR}/Documentation/InsightDoxygen.tag
  )

  FILE( COPY ${QT_DOC_DIR}/html/qt.tags
    DESTINATION ${GOFIGURE2_BINARY_DIR}/Documentation/
  )
ENDIF( LINK_EXTERNAL_DOC )

CONFIGURE_FILE (
  ${QGOIMAGECOMPARE_SOURCE_DIR}/Documentation/Doxygen/Doxyfile.txt.in
  ${QGOIMAGECOMPARE_BINARY_DIR}/Doxyfile.txt
)

ADD_CUSTOM_TARGET(doc ALL
  ${DOXYGEN_EXECUTABLE} ${QGOIMAGECOMPARE_BINARY_DIR}/Doxyfile.txt
  DEPENDS ${QGOIMAGECOMPARE_BINARY_DIR}/Doxyfile.txt
)

