#include <Python.h>

#include <iostream>

/** Our gdb pretty printer class.  An instantiation of this struct is like a C++
 * class this*, but in python it is traditionally call self*.
 */
typedef struct
{
  PyObject_HEAD
  PyObject * val; //  the gdb.Value to be 'printed'
} ICPPrinter;

/** This is equivalent in C++ to the constructor for ICPPrinter. */
static PyObject *
ICPPrinter_new( PyTypeObject *type, PyObject *args, PyObject *kwds )
{
  ICPPrinter *self;

  self = (ICPPrinter *)type->tp_alloc(type, 0);
  if ( self != NULL )
  {
  PyObject * gdbModule = PyImport_ImportModule( "gdb" );
  if( gdbModule == NULL )
    return NULL;
  PyObject * valueClass = PyObject_GetAttrString( gdbModule, "Value" );
  Py_DECREF( gdbModule );
  if( valueClass == NULL )
    return NULL;
  PyObject * valueArgs = Py_BuildValue( "(i)", 0 );
  self->val = PyEval_CallObject( valueClass, valueArgs );
  Py_DECREF( valueArgs );
  Py_DECREF( valueClass );
  if( self->val == NULL )
    {
    Py_DECREF( self );
    return NULL;
    }
  }

  return (PyObject *)self;
}

/** This is equivalent in C++ to the destructor for ICPPrinter. */
static void
ICPPrinter_dealloc( ICPPrinter * self )
{
  Py_XDECREF( self->val );
  self->ob_type->tp_free((PyObject*)self);
}


/** This defines the python constructor that would normally be defined with
 * __init__(self) in python. */
static int
ICPPrinter_init( ICPPrinter * self, PyObject * args, PyObject * kwds )
{
  PyObject * val = NULL;
  PyObject * tmp;

  static char *kwlist[] = { NULL };

  if ( !PyArg_ParseTupleAndKeywords( args, kwds, "O", kwlist,
      val ))
    return -1;

  if (val)
    {
    tmp = self->val;
    Py_INCREF(val);
    self->val = val;
    Py_XDECREF(tmp);
    }

  return 0;
}

/** The python class members for ICPPrinter. */
static PyMethodDef ICPPrinter_methods[] = {
    //{"to_string", (PyCFunction)ICPPrinter_to_string, METH_ARGS,
     //"Prints information about the image and sends it to the gui."
    //},
    {NULL}  /* Sentinel */
};


/** The PyTypeObject for ICPPrinter.  This defines how ICPPrinter behaves in
 * python. */
static PyTypeObject ICPPrinterType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "icp.ICPPrinter",             /*tp_name*/
    sizeof( ICPPrinter ),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)ICPPrinter_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "GDB Pretty Printer that creates a Qt/VTK gui displaying images passed to it.",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    ICPPrinter_methods,             /* tp_methods */
    0,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)ICPPrinter_init,      /* tp_init */
    0,                         /* tp_alloc */
    ICPPrinter_new,                 /* tp_new */
};

/** Create the lookup_function Python function.  This is a function associated
 * with the module expected by my version of gdb.pretty used for by register
 * function.
 *
 * The PyObject* is return, or NULL if a failure occurred.
 */
PyObject * lookup_function()
{
  PyObject * npArrayModule = PyImport_ImportModule( "itk.v3.numpy.array" );
  if( npArrayModule == NULL )
    return NULL;
  PyObject * lookupFunctionClass = PyObject_GetAttrString( npArrayModule, "RELookupFunctionTagNumpyType" );
  Py_DECREF( npArrayModule );
  if( lookupFunctionClass == NULL )
    return NULL;

  PyObject * prettyPrintersDict = PyDict_New();
  PyObject * prettyPrintersDictArgs = Py_BuildValue( "(O)", prettyPrintersDict );

  PyObject * lookupFunction = PyEval_CallObject( lookupFunctionClass,  prettyPrintersDictArgs );
  Py_DECREF( prettyPrintersDict );
  Py_DECREF( prettyPrintersDictArgs );
  Py_DECREF( lookupFunctionClass );
  if( lookupFunction == NULL )
    return NULL;

  return lookupFunction;
}

extern "C"
{

static PyObject * icpError;

static PyMethodDef icpMethods[] =
{
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyMODINIT_FUNC  initicp(void)
{
  // The module object
  PyObject * m;
  m = Py_InitModule3( "icp", icpMethods, "Use itkCompareProject to examine images while with the gdb's pretty printers." );
  if( m == NULL )
    return;

  // Create a module specific exception.  This will be used when encountering
  // itk::Exception's, etc.
  icpError = PyErr_NewException(const_cast<char*>("icp.error"), NULL, NULL);
  Py_INCREF(icpError);
  PyModule_AddObject( m, "error", icpError );

  // Create the ICPPrinter gdb pretty printer.
  if( PyType_Ready( &ICPPrinterType ) < 0 )
    return;
  Py_INCREF( &ICPPrinterType );
  PyModule_AddObject( m, "ICPPrinter", (PyObject *)&ICPPrinterType );

  // Create lookup_function.
  PyObject * lookupFunction = lookup_function();
  if( lookupFunction == NULL )
    return;
  PyModule_AddObject( m, "lookup_function", lookupFunction );
}
} // end extern "C"
