/*
Copyright (c) 2016, Dan Eicher
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jit-module.h"

static PyObject *
PyJit_label__get_is_valid(PyJit_label *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_valid());
}

static PyGetSetDef PyJit_label__getsets[] = {
    {
        (char*) "is_valid",                                       /* attribute name */
        (getter) PyJit_label__get_is_valid,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static PyObject *
PyJit_label__copy__(PyJit_label *self)
{
    PyJit_label *py_copy = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_copy->obj = new jit_label(*self->obj);

    return (PyObject *) py_copy;
}

static PyMethodDef PyJit_label_methods[] = {
    {(char *) "__copy__", (PyCFunction) PyJit_label__copy__, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static int
PyJit_label__tp_init(PyJit_label *self, PyObject *args, PyObject *kwargs)
{
    PyJit_label *copy = NULL;
    const char *keywords[] = {"ctor_arg", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "|O!", (char **) keywords, &PyJit_label_Type, &copy)) {
        return -1;
    }

    if (copy)
        self->obj = new jit_label(*((PyJit_label *)copy)->obj);
    else
        self->obj = new jit_label();

    return 0;
}

static void
PyJit_label__tp_dealloc(PyJit_label *self)
{
    delete self->obj;
    self->obj = NULL;

    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_label_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Label",                                         /* tp_name */
    sizeof(PyJit_label),                                          /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_label__tp_dealloc,                          /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)NULL,                                     /* tp_as_sequence */
    (PyMappingMethods*)NULL,                                      /* tp_as_mapping */
    (hashfunc)NULL,                                               /* tp_hash */
    (ternaryfunc)NULL,                                            /* tp_call */
    (reprfunc)NULL,                                               /* tp_str */
    (getattrofunc)NULL,                                           /* tp_getattro */
    (setattrofunc)NULL,                                           /* tp_setattro */
    (PyBufferProcs*)NULL,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                           /* tp_flags */
    NULL,                                                         /* Documentation string */
    (traverseproc)NULL,                                           /* tp_traverse */
    (inquiry)NULL,                                                /* tp_clear */
    (richcmpfunc)NULL,                                            /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)NULL,                                            /* tp_iter */
    (iternextfunc)NULL,                                           /* tp_iternext */
    (struct PyMethodDef*)PyJit_label_methods,                     /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_label__getsets,                                         /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_label__tp_init,                               /* tp_init */
    (allocfunc)PyType_GenericAlloc,                               /* tp_alloc */
    (newfunc)PyType_GenericNew,                                   /* tp_new */
    (freefunc)0,                                                  /* tp_free */
    (inquiry)NULL,                                                /* tp_is_gc */
    NULL,                                                         /* tp_bases */
    NULL,                                                         /* tp_mro */
    NULL,                                                         /* tp_cache */
    NULL,                                                         /* tp_subclasses */
    NULL,                                                         /* tp_weaklist */
    (destructor) NULL                                             /* tp_del */
};

