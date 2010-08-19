#include <Python.h>

#include <iostream>


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

  // Create lookup_function.
  PyObject * lookupFunction = lookup_function();
  if( lookupFunction == NULL )
    return;
  PyModule_AddObject( m, "lookup_function", lookupFunction );
}
} // end extern "C"
