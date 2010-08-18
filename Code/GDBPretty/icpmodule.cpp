#include <Python.h>

#include <iostream>

extern "C"
{

static PyObject * helloworld(PyObject *self, PyObject *args);
static PyObject * icpError;

static PyMethodDef icpMethods[] =
{
  {"helloworld", helloworld, METH_VARARGS,
"say hey yo"
  },
  {NULL, NULL, 0, NULL}
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

  // Create lookup_function.
  PyObject * npArrayModule = PyImport_ImportModule( "itk.v3.numpy.array" );
  if( npArrayModule == NULL )
    return;
  PyObject * lookupFunctionClass = PyObject_GetAttrString( npArrayModule, "RELookupFunctionTagNumpyType" );
  Py_DECREF( npArrayModule );
  if( lookupFunctionClass == NULL )
    return;

  PyObject * prettyPrintersDict = PyDict_New();
  PyObject * prettyPrintersDictArgs = Py_BuildValue( "(O)", prettyPrintersDict );

  PyObject * lookupFunction = PyEval_CallObject( lookupFunctionClass,  prettyPrintersDictArgs );
  Py_DECREF( prettyPrintersDict );
  Py_DECREF( prettyPrintersDictArgs );
  Py_DECREF( lookupFunctionClass );
  if( lookupFunction == NULL )
    return;

  PyModule_AddObject( m, "lookup_function", lookupFunction );
  Py_DECREF( lookupFunction );
}
} // end extern "C"

static PyObject *
helloworld( PyObject * self, PyObject * args )
{
  std::cout << "hello from inside the python module" << std::endl;
  return Py_BuildValue( "" ); // None
}
