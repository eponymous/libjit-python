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
PyJit_block__get_context(PyJit_block *self, void * UNUSED(closure))
{
    PyJit_context *retval;
    jit_context_t ctx = jit_block_get_context(self->obj);

    retval = PyObject_New(PyJit_context, &PyJit_context_Type);
    retval->obj = new jit_context(ctx);

    return (PyObject *)retval;
}

static PyObject *
PyJit_block__get_ends_in_dead(PyJit_block *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_block_ends_in_dead(self->obj));
}

static PyObject *
PyJit_block__get_function(PyJit_block *self, void * UNUSED(closure))
{
    jit_function_t retval = jit_block_get_function(self->obj);
    jit_function *func = jit_function::from_raw(retval);

    if (func == NULL) {
        func = new jit_function(retval);
    }

    return convert_c2py__jit_function(func, 1);
}

static PyObject *
PyJit_block__get_is_reachable(PyJit_block *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_block_is_reachable(self->obj));
}

static PyObject *
PyJit_block__get_label(PyJit_block *self)
{
    PyJit_label *py_jit_label = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_jit_label->obj = new jit_label(jit_block_get_label(self->obj));

    return (PyObject *)py_jit_label;
}

static PyGetSetDef PyJit_block__getsets[] = {
    {
        (char*) "context",                                        /* attribute name */
        (getter) PyJit_block__get_context,                        /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "ends_in_dead",                                   /* attribute name */
        (getter) PyJit_block__get_ends_in_dead,                   /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "function",                                       /* attribute name */
        (getter) PyJit_block__get_function,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_reachable",                                   /* attribute name */
        (getter) PyJit_block__get_is_reachable,                   /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "label",                                          /* attribute name */
        (getter) PyJit_block__get_label,                          /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static PyMethodDef PyJit_block_methods[] = {
    {NULL, NULL, 0, NULL}
};


static void
PyJit_block__tp_dealloc(PyJit_block *self)
{
    self->obj = NULL;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_block_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Block",                                         /* tp_name */
    sizeof(PyJit_block),                                          /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_block__tp_dealloc,                          /* tp_dealloc */
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
    (struct PyMethodDef*)PyJit_block_methods,                     /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_block__getsets,                                         /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)NULL,                                               /* tp_init */
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

static PyObject *
PyJit_block_list__tp_iter(PyJit_block_list *self)
{
    Py_INCREF(self);
    return (PyObject *) self;
}

static PyObject *
PyJit_block_list__tp_iternext(PyJit_block_list *self)
{
    PyJit_block *retval;

    jit_block_t next = jit_block_next(self->func->obj->raw(), (self->prev ? self->prev->obj : NULL));

    if (next == NULL) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    retval = PyObject_New(PyJit_block, &PyJit_block_Type);
    retval->obj = next;

    Py_XDECREF(self->prev);

    self->prev = retval;

    PY_XINCREF(self->prev);

    return (PyObject *)retval;
}

static PyObject *
PyJit_block_list_mp_subscript(PyObject *self, PyObject *key)
{
    PyJit_block *retval;
    jit_function_t func = ((PyJit_block_list *)self)->func->obj->raw();

    if (!PyInt_Check(key)) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    int index = PyInt_AsLong(key);

    jit_block_t block = (index >= 0 ? jit_block_next(func, NULL) : jit_block_previous(func, NULL));

    if (index > 0) {
        for (int i = index; i > 0 && block != NULL; i--) {
            block = jit_block_next(func, block);
        }
    } else if (index < 0) {
        for (int i = index; i < -1 && block != NULL; i++) {
            block = jit_block_previous(func, block);
        }
    }

    if (func == NULL) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    retval = PyObject_New(PyJit_block, &PyJit_block_Type);
    retval->obj = block;

    return (PyObject *)retval;
}

static PyMappingMethods PyJit_block_list_as_mapping = {
    NULL,                                                         /* mp_length */
    PyJit_block_list_mp_subscript,                                /* mp_subscript */
    NULL,                                                         /* mp_ass_subscript */
};

static void
PyJit_function_iter__tp_clear(PyJit_block_list *self)
{
    Py_CLEAR(self->func);
    Py_CLEAR(self->prev);
}

static void
PyJit_block_list__tp_dealloc(PyJit_block_list *self)
{
    PyJit_function_iter__tp_clear(self);

    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_block_list_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.BlockIter",                                     /* tp_name */
    sizeof(PyJit_block),                                          /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_block_list__tp_dealloc,                     /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)NULL,                                     /* tp_as_sequence */
    (PyMappingMethods*)&PyJit_block_list_as_mapping,              /* tp_as_mapping */
    (hashfunc)NULL,                                               /* tp_hash */
    (ternaryfunc)NULL,                                            /* tp_call */
    (reprfunc)NULL,                                               /* tp_str */
    (getattrofunc)NULL,                                           /* tp_getattro */
    (setattrofunc)NULL,                                           /* tp_setattro */
    (PyBufferProcs*)NULL,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                           /* tp_flags */
    NULL,                                                         /* Documentation string */
    (traverseproc)NULL,                                           /* tp_traverse */
    (inquiry)PyJit_function_iter__tp_clear,                       /* tp_clear */
    (richcmpfunc)NULL,                                            /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)PyJit_block_list__tp_iter,                       /* tp_iter */
    (iternextfunc)PyJit_block_list__tp_iternext,                  /* tp_iternext */
    (struct PyMethodDef*)NULL,                                    /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    NULL,                                                         /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)NULL,                                               /* tp_init */
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

