IF( WIN32 )
  IF( NOT CYGWIN )
    IF( NOT MINGW )
      IF( BUILD_SHARED_LIBS )
        ADD_DEFINITIONS( -DvtkRenderingAddOn2_EXPORTS )
      ENDIF( BUILD_SHARED_LIBS )
    ENDIF( NOT MINGW )
  ENDIF( NOT CYGWIN )
ENDIF( WIN32 )

CONFIGURE_FILE(${MEGAVTK2_SOURCE_DIR}/MegaVTK2Configure.h.in
               ${MEGAVTK2_BINARY_DIR}/MegaVTK2Configure.h @ONLY IMMEDIATE)

SUBDIRS( vtkRenderingAddOn )
SUBDIRS( vtkItk )
