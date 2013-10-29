PROJECT( QGOIMAGECOMPARE )

# CMake >= 2.8.2 MUST be installed (which is requiered by ITK)
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.2 )

IF( COMMAND CMAKE_POLICY )
  CMAKE_POLICY( VERSION 2.6 )
  CMAKE_POLICY( SET CMP0005 OLD )
  CMAKE_POLICY( SET CMP0003 NEW )
ENDIF(COMMAND CMAKE_POLICY)

# When making one release: update QGOIMAGECOMPARE_*_VERSION,
# QGOIMAGECOMPARE_WC_REVISION and the date of the release
SET( QGOIMAGECOMPARE_MAJOR_VERSION "0" )
SET( QGOIMAGECOMPARE_MINOR_VERSION "6" )
SET( QGOIMAGECOMPARE_WC_REVISION "0" )

SET( QGOIMAGECOMPARE_VERSION
  "${QGOIMAGECOMPARE_MAJOR_VERSION}.${QGOIMAGECOMPARE_MINOR_VERSION}.${QGOIMAGECOMPARE_WC_REVISION}" )

SET( QGOIMAGECOMPARE_LIB_VERSION
  "${QGOIMAGECOMPARE_MAJOR_VERSION}.${QGOIMAGECOMPARE_MINOR_VERSION}" )

MESSAGE( STATUS "QGoImageCompare version ${QGOIMAGECOMPARE_VERSION}" )

# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings. Copied from ITK CMakeLists.
IF( WIN32 )
  IF( NOT BORLAND )
    IF( NOT CYGWIN )
      IF( NOT MINGW )
          ADD_DEFINITIONS(
            -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
            -D_CRT_IS_WCTYPE_NO_DEPRECATE
            -D_CRT_MANAGED_FP_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
            -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
            -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
            -D_CRT_VCCLRIT_NO_DEPRECATE
            -D_SCL_SECURE_NO_DEPRECATE
            )
      ENDIF( NOT MINGW )
    ENDIF( NOT CYGWIN )
  ENDIF( NOT BORLAND )
ENDIF( WIN32 )

#-----------------------------------------------------------------------
# Export and Install configuration
#
# QGOIMAGECOMPARE_INSTALL_BUNDLE_DIR       - bundle dir
# QGOIMAGECOMPARE_INSTALL_BIN_DIR          - binary dir (executables)
# QGOIMAGECOMPARE_INSTALL_LIB_DIR          - library dir (libs)
# QGOIMAGECOMPARE_INSTALL_DOC_DIR          - documentation dir
# QGOIMAGECOMPARE_INSTALL_LICENSE_DIR      - licenses dir
#-----------------------------------------------------------------------
IF( NOT QGOIMAGECOMPARE_INSTALL_BUNDLE_DIR )
  SET( QGOIMAGECOMPARE_INSTALL_BUNDLE_DIR "." )
ENDIF( NOT QGOIMAGECOMPARE_INSTALL_BUNDLE_DIR )

IF( NOT QGOIMAGECOMPARE_INSTALL_BIN_DIR )
  SET( QGOIMAGECOMPARE_INSTALL_BIN_DIR "bin" )
ENDIF( NOT QGOIMAGECOMPARE_INSTALL_BIN_DIR )

IF( NOT QGOIMAGECOMPARE_INSTALL_LIB_DIR )
  SET( QGOIMAGECOMPARE_INSTALL_LIB_DIR "lib/gofigure2" )
ENDIF( NOT QGOIMAGECOMPARE_INSTALL_LIB_DIR )

IF( NOT QGOIMAGECOMPARE_INSTALL_DOC_DIR )
  SET( QGOIMAGECOMPARE_INSTALL_DOC_DIR "share/doc/gofigure2" )
ENDIF( NOT QGOIMAGECOMPARE_INSTALL_DOC_DIR )

IF( NOT QGOIMAGECOMPARE_INSTALL_LICENSE_DIR )
  SET( QGOIMAGECOMPARE_INSTALL_LICENSE_DIR "share/doc/gofigure2/Licenses" )
ENDIF( NOT QGOIMAGECOMPARE_INSTALL_LICENSE_DIR )

#---------------------------------------------------------------------------
# Define CMAKE_whatever_OUTPUT_DIRECTORY
#---------------------------------------------------------------------------
# all target executables will be
# in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${QGOIMAGECOMPARE_BINARY_DIR}/bin
)

# all target static libraries will be in ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${QGOIMAGECOMPARE_BINARY_DIR}/lib
)

# all shared libraries will be in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
# Note that modules (plugins) are considered as shared libraries
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY
#  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
   ${QGOIMAGECOMPARE_BINARY_DIR}/lib
)

#---------------------------------------------------------------------------
# Define TESTING_DATA_PATH
#---------------------------------------------------------------------------
SET( TESTING_DATA_PATH
  ${QGOIMAGECOMPARE_SOURCE_DIR}/Testing/Data
  CACHE PATH "Directory which contains data for testing."
)

#---------------------------------------------------------------------------
# Handle Documentation
#---------------------------------------------------------------------------
OPTION( BUILD_DOCUMENTATION "Build Doxygen Documentation" OFF )

#---------------------------------------------------------------------------
IF( BUILD_DOCUMENTATION )
  FIND_PACKAGE( Doxygen )
  IF( DOXYGEN_FOUND )
    IF( DOXYGEN_DOT_FOUND )

      OPTION( LINK_EXTERNAL_DOC
        "Should the doc be linked with external sources (VTK, ITK, Qt)?"
        OFF
      )

      ADD_SUBDIRECTORY( Documentation )

    ELSE( DOXYGEN_DOT_FOUND )
      MESSAGE( STATUS
        "Warning: Dot (from GraphViz) not found - Documentation will not ce generated"
      )
    ENDIF( DOXYGEN_DOT_FOUND )
  ELSE( DOXYGEN_FOUND )
    MESSAGE( STATUS
      "Warning: Doxygen not found - Documentation will not be created"
    )
  ENDIF( DOXYGEN_FOUND )
ENDIF( BUILD_DOCUMENTATION )

#---------------------------------------------------------------------------
# Determine if we want to build the gdb pretty printer python module.
#---------------------------------------------------------------------------

SET( BUILD_GDB_PRETTY OFF CACHE BOOL
  "Build a module for debugging with gdb pretty printers" )


#---------------------------------------------------------------------------
# Main GoFigure Settings
#---------------------------------------------------------------------------

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${QGOIMAGECOMPARE_SOURCE_DIR}/CMake/"
)

IF( UNIX )
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF( UNIX )

# Everything depends on VTK first
# that way we can get Qt info from VTK
#### VTK ####
FIND_PACKAGE( VTK REQUIRED )
IF( VTK_FOUND )

  # Check the version of VTK
  # QGoImageCompare requires vtk >= 5.6
  IF( ( ${VTK_MAJOR_VERSION} LESS 5 ) OR ( ${VTK_MINOR_VERSION} LESS 6 ) )
    MESSAGE( SEND_ERROR
      "QGoImageCompare requires VTK 5.6 (your version of VTK is ${VTK_VERSION})" )
  ENDIF( ( ${VTK_MAJOR_VERSION} LESS 5 ) OR ( ${VTK_MINOR_VERSION} LESS 6 ) )

  # Check if version of VTK is > 5.6
  # Requiered to now if we can use setBitRate() and setBitRateTolerance
  # in the vtkFFMPEGWriter
  IF( ( ${VTK_MAJOR_VERSION} EQUAL 5 ) AND ( ${VTK_MINOR_VERSION} GREATER 6 ))
    ADD_DEFINITIONS( -DVTKTRUNK )
  ENDIF( ( ${VTK_MAJOR_VERSION} EQUAL 5 ) AND ( ${VTK_MINOR_VERSION} GREATER 6 ))

  INCLUDE( ${VTK_USE_FILE} )

  #----------------------------------------------------------
  # Determine if GoFigure has to be built in shared or static
  # based on the configuration of VTK
  IF( VTK_BUILD_SHARED_LIBS )
    SET( LIBS_STYLE "SHARED" )
    SET( BUILD_SHARED_LIBS "TRUE" )
    ADD_DEFINITIONS( -DQGOIMAGECOMPARE_BUILD_SHARED_LIBS )
  ELSE( VTK_BUILD_SHARED_LIBS )
    SET( LIBS_STYLE "STATIC" )
    SET( BUILD_SHARED_LIBS "FALSE" )
    REMOVE_DEFINITIONS( -DQGOIMAGECOMPARE_BUILD_SHARED_LIBS )
  ENDIF( VTK_BUILD_SHARED_LIBS )
  #----------------------------------------------------------

  #----------------------------------------------------------
  # Then find ITK
  #### ITK ####
  FIND_PACKAGE( ITK REQUIRED )
  IF( ITK_FOUND )
    INCLUDE( ${ITK_USE_FILE} )

    IF( ( ${ITK_VERSION_MAJOR} LESS 3 ) OR
        ( ( ${ITK_VERSION_MAJOR} EQUAL 3 ) AND
          ( ${ITK_VERSION_MINOR} LESS 18 ) ) )
      MESSAGE( SEND_ERROR
        "QGoImageCompare requires ITK 3.18 or newer version (your version of ITK is ${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH})" )
    ENDIF( ( ${ITK_VERSION_MAJOR} LESS 3 ) OR
        ( ( ${ITK_VERSION_MAJOR} EQUAL 3 ) AND
          ( ${ITK_VERSION_MINOR} LESS 18 ) ) )


  IF( ${ITK_VERSION_MAJOR} EQUAL 3 )
    IF( ( ${ITK_USE_REVIEW} MATCHES "OFF" ) OR
        ( ${ITK_USE_REVIEW_STATISTICS} MATCHES "OFF" ) )
      MESSAGE( SEND_ERROR
        "QGoImageCompare requires ITK to be compiled with USE_REVIEW and USE_REVIEW_STATISTICS turned ON" )
     ENDIF( ( ${ITK_USE_REVIEW} MATCHES "OFF" ) OR
        ( ${ITK_USE_REVIEW_STATISTICS} MATCHES "OFF" ) )
  ELSE( ${ITK_VERSION_MAJOR} EQUAL 3 )
    IF( ${Module_ITKReview} MATCHES "OFF" )
      MESSAGE( SEND_ERROR
        "QGoImageCompare requires ITK to be compiled with USE_REVIEW turned ON" )
    ENDIF( ${Module_ITKReview} MATCHES "OFF" )

  ENDIF( ${ITK_VERSION_MAJOR} EQUAL 3 )

  ENDIF( ITK_FOUND )
  #----------------------------------------------------------

  #----------------------------------------------------------
  # Try to find Qt.
  # Note that some command line tools will get compiled even without Qt
  #### Qt ####
  FIND_PACKAGE( Qt4 REQUIRED )
  IF( QT4_FOUND AND QT_VERSION_MINOR GREATER 4 )
    IF( QT_USE_FILE )
      SET( QT_USE_QT3SUPPORT FALSE )
      SET( QT_USE_QTCORE TRUE )
      SET( QT_USE_QTGUI TRUE )
      INCLUDE( ${QT_USE_FILE} )
    ELSE( QT_USE_FILE )
      SET( QT_LIBRARIES ${QT_QT_LIBRARY} )
    ENDIF( QT_USE_FILE )

    SET( QT_PLUGIN_LIBS ${QT_LIBRARIES} )

    ADD_DEFINITIONS( ${QT_DEFINITIONS} )

    SET( QGOIMAGECOMPARE_EXTERNAL_SOURCE_DIR
      ${QGOIMAGECOMPARE_SOURCE_DIR}/Code/ExternalCode )
    SET( QGOIMAGECOMPARE_EXTERNAL_BINARY_DIR
      ${QGOIMAGECOMPARE_BINARY_DIR}/Code/ExternalCode )

    SET( MEGAVTK2_SOURCE_DIR ${QGOIMAGECOMPARE_EXTERNAL_SOURCE_DIR}/MegaVTK )
    SET( MEGAVTK2_BINARY_DIR ${QGOIMAGECOMPARE_EXTERNAL_BINARY_DIR}/MegaVTK )

    LINK_DIRECTORIES(
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      )


    INCLUDE_DIRECTORIES( BEFORE
      ${QT_INCLUDES}
      ${QGOIMAGECOMPARE_SOURCE_DIR}/Code/
      ${QGOIMAGECOMPARE_BINARY_DIR}/Code/
      ${QGOIMAGECOMPARE_SOURCE_DIR}/Code/GUI/
      ${QGOIMAGECOMPARE_BINARY_DIR}/Code/GUI/
      ${QGOIMAGECOMPARE_SOURCE_DIR}/Code/GUI/lib/
      ${QGOIMAGECOMPARE_BINARY_DIR}/Code/GUI/lib/
      ${QGOIMAGECOMPARE_BINARY_DIR}/Code/GUI/lib/Resources/
      ${MEGAVTK2_SOURCE_DIR}
      ${MEGAVTK2_BINARY_DIR}
      ${MEGAVTK2_SOURCE_DIR}/vtkItk
      ${MEGAVTK2_BINARY_DIR}/vtkItk
      ${MEGAVTK2_SOURCE_DIR}/vtkRenderingAddOn
      ${MEGAVTK2_BINARY_DIR}/vtkRenderingAddOn
    )

    # Try to extract Qt variable from VTK
    SET( QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "" )
    SET( QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "" )
    SET( QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "" )
    #---------------------------------------------------------------------

    ADD_SUBDIRECTORY( Code )

    #-----------------------------------------------------------------------
    # Examples and tests
    #-----------------------------------------------------------------------
    OPTION( BUILD_EXAMPLES "Build Examples" OFF )

    IF( BUILD_TESTING )
      SET( BUILDNAME "${BUILDNAME}"
             CACHE STRING "Name of build on the dashboard"
            )

      MARK_AS_ADVANCED( BUILDNAME )

      CONFIGURE_FILE( CMake/CTestCustom.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake
        @ONLY
        )
      INCLUDE ( CTest )
      ADD_SUBDIRECTORY( Testing )
    ENDIF( BUILD_TESTING )

    ADD_SUBDIRECTORY( Main )

    IF( BUILD_EXAMPLES )
      ADD_SUBDIRECTORY( Examples )
      OPTION( BUILD_TESTING "Test the project" ON )
    ELSE( BUILD_EXAMPLES )
      SET( BUILD_TESTING OFF )
    ENDIF( BUILD_EXAMPLES )

  ELSE( QT4_FOUND AND QT_VERSION_MINOR GREATER 4 )
    MESSAGE( SEND_ERROR "QGoImageCompare requires Qt4.5 (or newer version) to be installed" )
  ENDIF( QT4_FOUND AND QT_VERSION_MINOR GREATER 4 )

ENDIF( VTK_FOUND )
