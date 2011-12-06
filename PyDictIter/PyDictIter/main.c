#include <stdio.h>
#include <Python/Python.h>

PyObject* make_dict () {
    const char* values [] = {
        "foo",
        "bar",
        "baz"
    };
    PyObject* dict = PyDict_New();
    Py_INCREF(dict);
    for (int i = 0; i < 3; i++) {
        PyObject* key = PyInt_FromLong(i);
        PyObject* val = PyString_FromString(values[i]);
        
        PyDict_SetItem (dict, val, key);
        
        Py_XDECREF(key);
        Py_XDECREF(val);
    }
    return dict;
}

void* iterate (PyObject* dict,
               void*(*visitor)(PyObject*,PyObject*,void*),
               void* state) {
    PyObject* keys = PyDict_Keys(dict);
    PyObject* iter = PyObject_GetIter(keys);
    PyObject* key = NULL;
    void* result = NULL;
    
    while ((key = PyIter_Next(iter)) != NULL && !result) {
        PyObject* item = PyDict_GetItem(dict, key);
        result = visitor(key, item, state);
        Py_XDECREF(key);
    }
    
    Py_XDECREF(iter);
    Py_XDECREF(keys);
    
    return result;
}

static struct PyMethodDef My_Methods [] = {
    {NULL, NULL}
};

void init_module (const char* module) {
    Py_InitModule4(module, 
                   My_Methods,
                   NULL, NULL,
                   PYTHON_API_VERSION);
}

void set_object (const char* module,
                 const char* var,
                 PyObject* object) {
    PyObject* imported_module = PyImport_ImportModule(module);
    PyObject* module_dict = PyModule_GetDict(imported_module);
    PyDict_SetItemString(module_dict, var, object);
    //Py_XDECREF(module_dict);
    Py_XDECREF(imported_module);
}

PyObject* get_object (const char* module,
                      const char* var) {
    PyObject* imported_module = PyImport_ImportModule(module);
    PyObject* module_dict = PyModule_GetDict(imported_module);
    PyObject* object = PyDict_GetItemString(module_dict, var);
    Py_XDECREF(module_dict);
    Py_XDECREF(imported_module);
    return object;
}

void int_string_test (const char* str) {
    PyObject* num = PyInt_FromString(str, NULL, 10);
    if (num) {
        printf ("%s made a pyint\n", str);
        Py_XDECREF(num);
    } else {
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
        printf ("%s did not make a pyint\n", str);
    }
}

void float_string_test (const char* str) {
    PyObject* string = PyString_FromString(str);
    PyObject* num = PyFloat_FromString(string, NULL);
    if (num) {
        printf ("%s made a pyfloat\n", str);
        Py_XDECREF(num);
    } else {
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
        printf ("%s did not make a pyint\n", str);
    }
    Py_XDECREF(string);
}

void 
string_test () {
    const char *strings [] = {
        "10",
        "10.0",
        "ten"
    };
    for (int i = 0; i < 3; i++) {
        int_string_test (strings[i]);
        float_string_test (strings[i]);
    }
}

void* dict_printer (PyObject* key, PyObject* value, void* state) {
    PyObject* key_string = PyObject_Str(key);
    PyObject* value_string = PyObject_Str(value);
    printf ("%s -> %s\n", PyString_AsString(key_string), PyString_AsString(value_string));
    Py_DECREF(key_string);
    Py_DECREF(value_string);
    return NULL;
}

int main (int argc, const char * argv[]) {
    const char* module = "my_module";
    const char* var = "my_var";
    Py_Initialize ();
    /**/
    init_module(module);
    PyObject* dict = make_dict();
    set_object(module, var, dict);
    iterate(get_object(module, var), dict_printer, NULL);
    Py_XDECREF(dict);
    /**/
    //string_test ();
    Py_Finalize();
    return 0;
}
