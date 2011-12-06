/*
 *  foo.c
 *  PyRepl++
 *
 *  Created by Dave on 18/11/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "foo.h"
#include <Python/Python.h>

typedef struct { /* Subtype of IOobject */
    PyObject_HEAD
} FooObject;

static PyObject*
Foo_Foo (PyObject* self, PyObject* args) {
    return PyString_FromString("FOO");
}

static struct PyMethodDef FooObject_Methods [] = {
    {"foo",     (PyCFunction)Foo_Foo, METH_VARARGS},
    {NULL, NULL}
};

static void
Foo_Dealloc (PyObject* self) {
    PyObject_Del(self);
}

static PyTypeObject FooType = {
    PyObject_HEAD_INIT(NULL) 
    0,
    "foo.Foo",                  /*tp_name*/
    sizeof(FooObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Foo_Dealloc,    /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr */
    0,                          /*tp_setattr */
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro */
    0,                          /*tp_setattro */
    0,                          /*tp_as_buffer */
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    0,                          /*tp_doc */
    0,                          /*tp_traverse */
    0,                          /*tp_clear */
    0,                          /*tp_richcompare */
    0,                          /*tp_weaklistoffset */
    PyObject_SelfIter,          /*tp_iter */
    NULL,                       /*tp_iternext */
    FooObject_Methods,          /*tp_methods */
    NULL,                       /*tp_members */
    NULL,                       /*tp_getset */
};

static PyObject*
newFoo (PyObject* unused, PyObject* args) {
    return (PyObject*)PyObject_New (FooObject, &FooType);
}

static struct PyMethodDef Foo_Methods [] = {
    {"Foo",  (PyCFunction)newFoo, METH_VARARGS, 0},
    {NULL, NULL}
};

void initFooModule () {
    Py_InitModule4 ("foo", 
                    Foo_Methods,
                    0,
                    (PyObject*)NULL,
                    PYTHON_API_VERSION);
}