#include <stdio.h>
#include <Python/Python.h>

static PyObject*
getStringIO () {
    PyObject *module = PyImport_ImportModule("StringIO");
    PyObject *module_dict = PyModule_GetDict(module);
    PyObject *constructor = PyDict_GetItemString(module_dict, "StringIO");
    
    PyObject *buffer = PyObject_CallFunction(constructor, NULL);
    
    return buffer;
}

static const char*
getValue (PyObject* buffer) {
    PyObject* data = PyObject_CallMethod(buffer, "getvalue", NULL);
    char* value = PyString_AsString(data);
    Py_DECREF(data);
    return value;
}

static void
writeData (PyObject* buffer, PyObject* data) {
    PyObject* method = PyString_FromString("write");
    PyObject_CallMethodObjArgs(buffer, method, data, NULL);
    Py_DECREF(method);
}

static void
writeString (PyObject* buffer, const char* data) {
    PyObject *object = PyString_FromString(data);
    writeData (buffer, object);
    Py_DECREF(object);
}

int main (int argc, const char * argv[]) {
    Py_Initialize ();
    
    PyObject* buffer = getStringIO ();
    writeString (buffer, "foo");
    writeString (buffer, "bar");
    writeString (buffer, "baz");
    printf ("Buffer: %s\n", getValue (buffer));
    Py_Finalize();
    return 0;
}
