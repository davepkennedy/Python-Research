#include <stdio.h>
#include <Python/Python.h>

typedef struct {
    PyObject_HEAD
    PyObject *stuff;
    PyObject *attrs;
} Foo;

static PyObject*
Foo_Alloc (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    return type->tp_alloc(type, 0);
}

static int
Foo_Init (PyObject *self, PyObject *args, PyObject *kwds) {
    ((Foo*)self)->stuff = PyList_New(0);
    ((Foo*)self)->attrs = PyDict_New();
    return 0;
}

static void
Foo_Dealloc (PyObject *self) {
    Py_XDECREF (((Foo*)self)->stuff);
    Py_XDECREF (((Foo*)self)->attrs);
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

static PyObject*
Foo_GetAttr (Foo* self, PyObject* name) {
    return PyDict_GetItem(self->attrs, name);
}

static int
Foo_SetAttr (Foo* self, PyObject* name, PyObject* attr) {
    return PyDict_SetItem (self->attrs, name, attr);
    
}

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
    .tp_new         = Foo_Alloc,
    //.tp_getattro    = (getattrofunc)Foo_GetAttr,
	//.tp_setattro    = (setattrofunc)Foo_SetAttr,
    //.tp_iter        = PyObject_SelfIter
};

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

int main (int argc, const char * argv[]) {
    Py_Initialize();
    initFoo();
    
    PyRun_SimpleString ("import test\n" \
                        "f = test.Foo()\n" \
                        /*"f.bar = 'baz'\n" \
                         "print f.bar\n" \
                         "print dir (f)\n" \ */
                        "class Bar (test.Foo):\n" \
                        "   def __init__ (self, name):\n" \
                        "      self.name = name\n" \
                        "   def doit (self):\n" \
                        "      print 'did it'\n" \
                        "b = Bar('jim')\n" \
                        "print dir (b)\n" \
                        "b.doit()");
    
    Py_Finalize();
    return 0;
}
