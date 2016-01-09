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
PyJit_context__get_meta_numeric(PyJit_context *self, void *closure)
{
    return PyInt_FromLong(jit_context_get_meta_numeric(self->obj->raw(), (jit_nuint)closure));
}

static int
PyJit_context__set_meta_numeric(PyJit_context *self, PyObject *value, void *closure)
{
    int i;

    PyObject *tmp = Py_BuildValue((char *) "(O)", value);
    if (!PyArg_ParseTuple(tmp, (char *) "i", &i)) {
        Py_DECREF(tmp);
        return -1;
    }
    Py_DECREF(tmp);

    jit_context_set_meta_numeric(self->obj->raw(), (jit_nuint)closure, i);

    return 0;
}

static PyObject *
PyJit_context__get_functions(PyJit_context *self, void *UNUSED(closure))
{
    PyJit_function_list *iter = PyObject_GC_New(PyJit_function_list, &PyJit_function_list_Type);

    Py_INCREF(self);
    iter->context = self;

    iter->prev = NULL;

    return (PyObject*) iter;
}

static PyGetSetDef PyJit_context__getsets[] = {
    {
        (char*) "cache_limit",                                    /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_CACHE_LIMIT                            /* optional additional data for getter and setter */
    },
    {
        (char*) "cache_page_size",                                /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_CACHE_PAGE_SIZE                        /* optional additional data for getter and setter */
    },
    {
        (char*) "pre_compile",                                    /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_PRE_COMPILE                            /* optional additional data for getter and setter */
    },
    {
        (char*) "dont_fold",                                      /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_DONT_FOLD                              /* optional additional data for getter and setter */
    },
    {
        (char*) "position_independent",                           /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_POSITION_INDEPENDENT                   /* optional additional data for getter and setter */
    },
    {
        (char*) "max_page_factor",                                /* attribute name */
        (getter) PyJit_context__get_meta_numeric,                 /* C function to get the attribute */
        (setter) PyJit_context__set_meta_numeric,                 /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        (void *)JIT_OPTION_CACHE_MAX_PAGE_FACTOR                  /* optional additional data for getter and setter */
    },
    {
        (char*) "functions",                                      /* attribute name */
        (getter) PyJit_context__get_functions,                    /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static PyObject *
PyJit_context_build_start(PyJit_context *self)
{
    self->obj->build_start();

    Py_RETURN_NONE;
}

static PyObject *
PyJit_context_build_end(PyJit_context *self)
{
    self->obj->build_end();

    Py_RETURN_NONE;
}

static PyMethodDef PyJit_context_methods[] = {
    {(char *) "build_start",    (PyCFunction) PyJit_context_build_start,    METH_NOARGS, NULL },
    {(char *) "build_end",      (PyCFunction) PyJit_context_build_end,      METH_NOARGS, NULL },
    {NULL, NULL, 0, NULL}
};

static int
PyJit_context__tp_init(PyJit_context *self, PyObject *args, PyObject *kwargs)
{
    const char *keywords[] = {NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return -1;
    }

    self->obj = new jit_context();

    return 0;
}

static void
PyJit_context__tp_dealloc(PyJit_context *self)
{
    delete self->obj;
    self->obj = NULL;

    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_context_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Context",                                       /* tp_name */
    sizeof(PyJit_context),                                        /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_context__tp_dealloc,                        /* tp_dealloc */
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
    (struct PyMethodDef*)PyJit_context_methods,                   /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_context__getsets,                                       /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_context__tp_init,                             /* tp_init */
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

