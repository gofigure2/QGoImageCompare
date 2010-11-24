SET( QGOGUILIB_EXAMPLE_SRC
  pipevtkexample
  pipeitkexample
)


FOREACH( var ${QGOGUILIB_EXAMPLE_SRC} )

  ADD_EXECUTABLE( ${var} ${var} )
  TARGET_LINK_LIBRARIES( ${var}
    QGoGui
  )

ENDFOREACH( var ${QGOGUILIB_EXAMPLE_SRC} )

