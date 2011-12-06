#include <stdio.h>
#include <Python/Python.h>

int main (int argc, const char * argv[]) {
    Py_Initialize();
    PyObject* module = PyImport_ImportModule("hashlib");
    PyObject* dict = PyModule_GetDict(module);
    PyObject* md5_cons = PyDict_GetItemString(dict, "md5");
    PyObject* md5 = PyObject_CallFunction(md5_cons, "");
    PyObject_CallMethod(md5, "update", "s", "this is a test");
    PyObject* hash = PyObject_CallMethod(md5, "hexdigest", "");
    if (hash) {
        char *hashString = PyString_AsString(hash);
        printf ("%s\n", hashString);
    }
    Py_XDECREF (hash);
    Py_XDECREF (md5);
    
    Py_Finalize();
    return 0;
}
