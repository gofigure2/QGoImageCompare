CONFIGURE_FILE ( 
  ${GOFIGURE2_SOURCE_DIR}/Documentation/Doxygen/Doxyfile.txt.in 
  ${GOFIGURE2_BINARY_DIR}/Doxyfile.txt 
) 

IF( LINK_EXTERNAL_DOC )
#  EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
#    ARGS -E tar xvz ${GOFIGURE2_SOURCE_DIR}/Documentation/Doxygen/vtkNightlyDoc.tag.tar.gz 
#)
#
#  EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
#    ARGS -E tar xvz ${GOFIGURE2_SOURCE_DIR}/Documentation/Doxygen/InsightDoxygen.tag.tar.gz 
#)
#
#  EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
#    ARGS -E tar xvz ${GOFIGURE2_SOURCE_DIR}/Documentation/Doxygen/qt.tag.tar.gz 
#)
  
ELSE( LINK_EXTERNAL_DOC )
   EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
    ARGS -E remove ${GOFIGURE2_BINARY_DIR}/Documentation/vtkNightlyDoc.tag )

   EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
    ARGS -E remove ${GOFIGURE2_BINARY_DIR}/Documentation/InsightDoxygen.tag 
)

   EXEC_PROGRAM( ${CMAKE_COMMAND} ${GOFIGURE2_BINARY_DIR}/Documentation/
    ARGS -E remove ${GOFIGURE2_BINARY_DIR}/Documentation/qt.tag 
)

ENDIF( LINK_EXTERNAL_DOC )

ADD_CUSTOM_TARGET(doc ALL 
  ${DOXYGEN_EXECUTABLE} ${GOFIGURE2_BINARY_DIR}/Doxyfile.txt 
  DEPENDS ${GOFIGURE2_BINARY_DIR}/Doxyfile.txt 
) 

