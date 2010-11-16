import gdb.pretty
gdb.pretty.register( 'icp',
        gdb.current_objfile() )
