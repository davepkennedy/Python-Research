/*
 *  bar.c
 *  PyRepl++
 *
 *  Created by Dave on 18/11/2011.
 *  Copyright 2011 __BarMyCompanyName__. All rights reserved.
 *
 */

#include "bar.h"
#include <Python/Python.h>

typedef struct { /* Subtype of IOobject */
    PyObject_HEAD
} BarObject;

static PyObject*
Bar_Bar (PyObject* self, PyObject* args) {
    return PyString_FromString("bar");
}

static struct PyMethodDef BarObject_Methods [] = {
    {"Bar",     (PyCFunction)Bar_Bar, METH_VARARGS},
    {NULL, NULL}
};

static void
Bar_Dealloc (PyObject* self) {
    PyObject_Del(self);
}

static PyTypeObject BarType = {
    PyObject_HEAD_INIT(NULL) 
    0,
    "bar.Bar",                  /*tp_name*/
    sizeof(BarObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Bar_Dealloc,    /*tp_dealloc*/
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
    BarObject_Methods,          /*tp_methods */
    NULL,                       /*tp_members */
    NULL,                       /*tp_getset */
};

static PyObject*
newBar (PyObject* unused, PyObject* args) {
    return (PyObject*)PyObject_New (BarObject, &BarType);
}

static struct PyMethodDef Bar_Methods [] = {
    {"Bar",  (PyCFunction)newBar, METH_VARARGS, 0},
    {NULL, NULL}
};

void initBarModule () {
    Py_InitModule4 ("bar", 
                    Bar_Methods,
                    0,
                    (PyObject*)NULL,
                    PYTHON_API_VERSION);
}