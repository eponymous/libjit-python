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
#include <jit/jit-dynamic.h>

typedef struct {
    PyObject_HEAD
    jit_dynlib_handle_t obj;
} PyJit_dynlib_handle;

extern PyTypeObject PyJit_dynlib_handle_Type;

static PyObject *
PyJit_dynamic_set_debug(PyObject * UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_flag;
    const char *keywords[] = {"flag", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O:set_debug", (char **) keywords, &py_flag)) {
        return NULL;
    }

    jit_dynlib_set_debug(PyObject_IsTrue(py_flag));

    Py_RETURN_NONE;
}

static PyObject *
PyJit_dynamic_get_suffix()
{
    return Py_BuildValue((char *) "s", jit_dynlib_get_suffix());
}

static PyMethodDef jit_dynamic_functions[] = {
    {(char *) "set_debug",  (PyCFunction) PyJit_dynamic_set_debug,    METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "get_suffix", (PyCFunction) PyJit_dynamic_get_suffix,   METH_NOARGS,                NULL },
    {NULL, NULL, 0, NULL}
};

static PyObject *
PyJit__dynamic_get_symbol(PyJit_dynlib_handle *self, PyObject *args, PyObject *kwargs)
{
    char const *name;
    const char *keywords[] = {"name", NULL};
    void *symbol;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s:get_symbol", (char **) keywords, &name)) {
        return NULL;
    }

    if ((symbol = jit_dynlib_get_symbol(self->obj, name)) == NULL) {
        PyErr_Format(PyExc_ValueError, "symbol '%s' not found", name);
        return NULL;
    }

    return PyLong_FromVoidPtr(symbol);
}

static PyMethodDef PyJit_dynlib_handle_methods[] = {
    {(char *) "get_symbol", (PyCFunction) PyJit__dynamic_get_symbol, METH_KEYWORDS|METH_VARARGS, NULL },
    {NULL, NULL, 0, NULL}
};

static int
PyJit_dynlib_handle__tp_init(PyJit_dynlib_handle *self, PyObject *args, PyObject *kwargs)
{
    char const *name;
    const char *keywords[] = {"name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &name)) {
        return -1;
    }

    self->obj = jit_dynlib_open(name);

    if (self->obj == NULL) {
        PyErr_Format(PyExc_ValueError, "unable to open library '%s'", name);
        return -1;
    }

    return 0;
}

static void
PyJit_dynlib_handle__tp_dealloc(PyJit_dynlib_handle *self)
{
    jit_dynlib_handle_t tmp = self->obj;
    self->obj = NULL;

    if (tmp) {
        jit_dynlib_close(tmp);
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}


PyTypeObject PyJit_dynlib_handle_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.dynamic.Library",                               /* tp_name */
    sizeof(PyJit_dynlib_handle),                                  /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_dynlib_handle__tp_dealloc,                  /* tp_dealloc */
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
    (struct PyMethodDef*)PyJit_dynlib_handle_methods,             /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    0,                                                            /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_dynlib_handle__tp_init,                       /* tp_init */
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

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_dynamic_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.dynamic",
    NULL,
    -1,
    jit_dynamic_functions,
};
#endif

PyObject *
initjit_dynamic(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_dynamic_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.dynamic", jit_dynamic_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }

    /* Register the 'jit_dynlib_handle_t' class */
    if (PyType_Ready(&PyJit_dynlib_handle_Type)) {
        return NULL;
    }

    PyModule_AddObject(m, (char *) "Library", (PyObject *) &PyJit_dynlib_handle_Type);

    return m;
}

