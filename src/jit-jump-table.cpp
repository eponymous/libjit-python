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


static Py_ssize_t
Jit_jump_table__sq_length (PyJit_jump_table *self)
{
    return self->obj->size();
}

static PyObject *
Jit_jump_table__sq_item(PyJit_jump_table *self, Py_ssize_t index)
{
    PyJit_label *py_jit_label;
    jit_label retval;

    if (index < 0)
        index += self->obj->size();

    try {
        retval = self->obj->get(index);
    }
    catch (jit_build_exception &e){
        PyErr_SetString(PyExc_IndexError, "JumpTable index out of range");
        return NULL;
    }

    py_jit_label = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_jit_label->obj = new jit_label(retval);

    return (PyObject *)py_jit_label;
}

static int
Jit_jump_table__sq_ass_item(PyJit_jump_table *self, Py_ssize_t index, PyObject *value)
{
    if (index < 0)
        index += self->obj->size();

    if (PyObject_IsInstance(value, (PyObject *)&PyJit_label_Type) == 0) {
        PyErr_SetString(PyExc_TypeError, "JumpTable can only hold jit.Label types.");
        return -1;
    }

    try {
        self->obj->set(index, *((PyJit_label *) value)->obj);
    }
    catch (jit_build_exception &e){
        PyErr_SetString(PyExc_IndexError, "JumpTable index out of range");
        return -1;
    }

    return 0;
}

static PySequenceMethods Jit_jump_table__py_sequence_methods = {
    (lenfunc) Jit_jump_table__sq_length,
    (binaryfunc) NULL,
    (ssizeargfunc) NULL,
    (ssizeargfunc) Jit_jump_table__sq_item,
    NULL,
    (ssizeobjargproc) Jit_jump_table__sq_ass_item,
    NULL,
    (objobjproc) NULL,
    /* Added in release 2.0 */
    (binaryfunc) NULL,
    (ssizeargfunc) NULL,
};

static int
PyJit_jump_table__tp_init(PyJit_jump_table *self, PyObject *args, PyObject *kwargs)
{
    int size;
    const char *keywords[] = {"size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &size)) {
        return -1;
    }
    self->obj = new jit_jump_table(size);
    return 0;
}

static void
PyJit_jump_table__tp_dealloc(PyJit_jump_table *self)
{
    delete self->obj;
    self->obj = NULL;

    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyTypeObject PyJit_jump_table_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.JumpTable",                                     /* tp_name */
    sizeof(PyJit_jump_table),                                     /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_jump_table__tp_dealloc,                     /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)&Jit_jump_table__py_sequence_methods,     /* tp_as_sequence */
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
    (struct PyMethodDef*)NULL,                                    /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    0,                                                            /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_jump_table__tp_init,                          /* tp_init */
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

