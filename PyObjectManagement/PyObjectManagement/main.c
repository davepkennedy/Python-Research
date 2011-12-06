#include <stdio.h>
#include <Python/Python.h>

typedef struct {
    PyObject_HEAD
    PyObject *stuff;
} Foo;

static PyObject*
Foo_Alloc (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    return type->tp_alloc(type, 0);
}

static int
Foo_Init (PyObject *self, PyObject *args, PyObject *kwds) {
    ((Foo*)self)->stuff = PyList_New(0);
    return 0;
}

static void
Foo_Dealloc (PyObject *self) {
    Py_XDECREF (((Foo*)self)->stuff);
}

static PyMethodDef
Foo_Methods [] = {
    {NULL}
};

static PyObject*
Foo_GetStuff (Foo* self, void* closure) {
    return self->stuff;
}

static int
Foo_SetStuff (Foo* self, PyObject *stuff, void *closure) {
    Py_XDECREF (self->stuff);
    Py_INCREF (stuff);
    self->stuff = stuff;
    
    return 0;
}

static PyGetSetDef 
Foo_GetSetters [] = {
    {"stuff",   (getter)Foo_GetStuff, (setter)Foo_SetStuff}
};

static PyTypeObject Foo_Type = {
    PyObject_HEAD_INIT (NULL)
    .ob_size        = 0,
    .tp_name        = "test.Foo",
    .tp_basicsize   = sizeof (Foo),
    .tp_dealloc     = (destructor)Foo_Dealloc,
    .tp_flags       = Py_TPFLAGS_DEFAULT |Py_TPFLAGS_BASETYPE,
    .tp_doc         = "variable storage",
    .tp_methods     = Foo_Methods,
    .tp_members     = 0,
    .tp_getset      = Foo_GetSetters,
    .tp_init        = (initproc)Foo_Init,
    .tp_new         = Foo_Alloc
};

int 
Foo_Check (PyObject *object) {
    return PyObject_IsInstance(object, (PyObject*)&Foo_Type) == 1;
}

static PyMethodDef Module_Methods [] = {
    {NULL}
};

void
initFoo () {
    PyObject *m;
    
    if (PyType_Ready(&Foo_Type) < 0) {
        return;
    }
    
    m = Py_InitModule3 ("test",
                        Module_Methods,
                        NULL);
    if (NULL == m) {
        return;
    }
    
    Py_INCREF(&Foo_Type);
    PyModule_AddObject(m, "Foo", (PyObject*)&Foo_Type);
    
    
}

static PyObject*
getModuleDict () {
    PyObject* module = PyImport_ImportModule("test");
    PyObject* dict = PyModule_GetDict(module);
    
    return dict;
}

static void
setModuleObject (const char* name, PyObject* object) {
    PyObject* dict = getModuleDict ();
    PyDict_SetItemString(dict, name, object);
}

static PyObject*
getModuleObject (const char* name) {
    PyObject* dict = getModuleDict ();
    PyObject* key = PyString_FromString(name);
    PyObject* foo = NULL;
    
    if (PyDict_Contains(dict, key)) {
        foo = PyDict_GetItem(dict, key);
    }
    Py_XDECREF(key);
    return foo;
}

void
printCompare (PyObject* obj) {
    printf ("%d\n", Foo_Check (obj));
}

void showStuff(PyObject *foo) {
    //foo = getModuleObject ();
    if (PyList_Check (((Foo*)foo)->stuff)) {
        printf ("len(stuff) -> %d\n", PyList_Size(((Foo*)foo)->stuff));
        for (int i = 0; i < PyList_Size(((Foo*)foo)->stuff); i++) {
            PyObject* item = PyList_GetItem(((Foo*)foo)->stuff, i);
            PyObject* str = PyObject_Str(item);
            char* str_value = PyString_AsString(str);
            printf ("%s\n", str_value);
            Py_XDECREF(str);
        }
    } else {
        printf ("foo is not a list!\n");
    }
}

void 
setValue (PyObject *obj, const char *name, PyObject *value) {
    PyObject *keyObj = PyString_FromString(name);
    long result = PyDict_SetItem(obj, keyObj, value);
    Py_DECREF(keyObj);
}

void
setPtrValue (PyObject *obj, const char *name, void *value) {
    PyObject* cobj = PyCObject_FromVoidPtr(value, NULL);
    setValue(obj, name, cobj);
    Py_DECREF(cobj);
}

void 
setStringValue (PyObject *obj, const char *name, const char *value) {
    PyObject *valObj = PyString_FromString(value);
    setValue (obj, name, valObj);
    Py_DECREF(valObj);
}

const char*
getStringValue (PyObject *obj, const char *name) {
    PyObject *keyObj = PyString_FromString(name);
    PyObject *valObj = PyObject_GetAttr(obj, keyObj);
    char * value = PyString_AsString(valObj);
    Py_DECREF(keyObj);
    Py_DECREF(valObj);
    
    return value;
}

int main (int argc, const char * argv[]) {
    PyObject* foo;
    
    Py_Initialize();
    union {
        int i;
        float f;
    } onion;
    
    onion.i = 100;
    
    initFoo();
    foo = Foo_Alloc((PyTypeObject*)&Foo_Type, NULL, NULL);
    Foo_Init(foo, NULL, NULL);
    setModuleObject("foo", foo);
    
    printCompare (foo);
    printCompare (getModuleDict());
    PyObject* type = PyObject_Type((PyObject*)foo);
    //PyRun_SimpleString ("import test\ntest.foo.stuff = [1,2,'monkey',4]");
    showStuff(foo);
    
    setStringValue(getModuleDict(), "biff", "boff");
    setPtrValue(getModuleDict(), "onion", &onion);
    PyRun_SimpleString ("import test\nprint '\\n'.join ([str((i, getattr(test, str(i)))) for i in dir(test)])");
    
    /*
     setStringValue (foo, "a", "foo");
     setStringValue (foo, "b", "bar");
     setStringValue (foo, "c", "baz");
     
     printf ("%s\n", getStringValue(foo, "a"));
     printf ("%s\n", getStringValue(foo, "b"));
     printf ("%s\n", getStringValue(foo, "c"));
     */
    
    //PyRun_SimpleString ("print dir (test.foo)");
    
    Py_Finalize();
    return 0;
}
