#include <stdio.h>
#include <Python/Python.h>

typedef struct {
    PyObject_HEAD
    char *value;
} Foo;

static PyObject*
Foo_Alloc (PyTypeObject* type, PyObject *args, PyObject *kwds) {
    return type->tp_alloc(type, 0);
}

static int
Foo_Init (Foo* self, PyObject *args, PyObject *kwds) {
    self->value = NULL;
    return 0;
}

static void
Foo_Dealloc (Foo *self) {
    printf ("Deleting %s\n", self->value ? self->value : "unknown");
    free (self->value);
    self->ob_type->tp_free(self);
}

static PyObject*
Foo_Repr (Foo* self) {
    return PyString_FromString(self->value ? self->value : "unknown");
}

static PyTypeObject
Foo_Type = {
    PyObject_HEAD_INIT(NULL)
    .ob_size        = 0,
    .tp_name        = "foo.Foo",
    .tp_basicsize   = sizeof(Foo),
    .tp_dealloc     = (destructor)Foo_Dealloc,
    .tp_flags       = Py_TPFLAGS_DEFAULT |Py_TPFLAGS_BASETYPE,
    .tp_init        = (initproc)Foo_Init,
    .tp_new         = Foo_Alloc,
    .tp_repr        = (reprfunc)Foo_Repr
};

static PyMethodDef
Module_Methods [] = {
    {NULL}
};

void
initFoo () {
    PyObject *m;
    
    if (PyType_Ready (&Foo_Type) < 0) {
        return;
    }
    
    m = Py_InitModule3 ("foo",
                        Module_Methods,
                        NULL);
    if (NULL == m) {
        return;
    }
    Py_INCREF (&Foo_Type);
    PyModule_AddObject(m, "Foo", (PyObject*)&Foo_Type);
}

static PyObject*
getModuleDict () {
    PyObject *module = PyImport_ImportModule("foo");
    PyObject *dict = PyModule_GetDict(module);
    
    return dict;
}

static PyObject*
getModuleArray () {
    PyObject *dict = getModuleDict();
    PyObject *key = PyString_FromString("my_list");
    PyObject *array = NULL;
    
    if (PyDict_Contains(dict, key)) {
        array = PyDict_GetItem(dict, key);
    } else {
        array = PyList_New(0);
        PyDict_SetItem(dict, key, array);
    }
    return array;
}

static void
deleteModuleArray () {
    PyObject *dict = getModuleDict();
    PyObject *key = PyString_FromString("my_list");
    PyObject *array = getModuleArray();
    
    if (PyDict_Contains(dict, key)) {
        PyDict_DelItem(dict, key);
    }
    Py_XDECREF(array);
}

Foo* foos [5];

void
addItem (int i) {
    char buffer [10];
    PyObject* array = getModuleArray ();
    
    sprintf (buffer, "%d", i);
    Foo* foo = (Foo*)Foo_Alloc(&Foo_Type, NULL, NULL);
    Foo_Init(foo, NULL, NULL);
    foo->value = strdup (buffer);
    
    PyList_Append(array, (PyObject*)foo);
    Py_DECREF(foo);
    foos[i] = foo;
    printf ("List size: %d\n", PyList_Size(array));
}

int main (int argc, const char * argv[]) {
    Py_Initialize ();
    
    initFoo ();
    for (int i = 0; i < 5; i++) {
        addItem (i);
    }
    //PyRun_SimpleString ("import foo\nprint foo.my_list");
    //PyRun_SimpleString ("import foo\nfoo.my_list = []");
    //PyRun_SimpleString ("import foo\nprint foo.my_list");
    //PyRun_SimpleString ("import foo\nprint foo.Foo()");
    deleteModuleArray ();
    
    Py_Finalize();
    return 0;
}