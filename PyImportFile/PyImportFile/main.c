#include <stdio.h>
#include <Python/Python.h>

#define MAX_PATH 255

void get_path (const char* filepath, char* buffer, size_t bufsz) {
    strncpy (buffer, filepath, bufsz);
    char* lastidx = strrchr(buffer, '/');
    *lastidx = 0;
}

void get_file (const char* filepath, char* buffer, size_t bufsz) {
    char* lastidx = strrchr (filepath, '/');
    strncpy (buffer, lastidx+1, bufsz);
}

void addToSysPath (const char* path) {
    PyObject * sysPath = PySys_GetObject("path");
    if (sysPath) {
        PyObject* pathObj = PyUnicode_FromString(path);
        PyList_Append(sysPath, pathObj);
        Py_DECREF(pathObj);
    }
}

void process_argument (const char* arg, const char* methodName) {
    char buffer [MAX_PATH];
    
    Py_Initialize ();
    get_path (arg, buffer, MAX_PATH);
    addToSysPath(buffer);
    
    get_file (arg, buffer, MAX_PATH);
    char* lastidx = strrchr(buffer, '.');
    if (lastidx) {
        *lastidx = 0;
    }
    
    PyObject * module = PyImport_ImportModule(buffer);
    
    PyObject* method = PyString_FromString(methodName);
    PyObject_CallMethodObjArgs(module, method, NULL);
    Py_DECREF (methodName);
    
    Py_Finalize ();
}

int main (int argc, const char * argv[]) {
    for (int i = 1; i < argc; i+=2) {
        process_argument (argv[i], argv[i+1]);
    }
    return 0;
}
