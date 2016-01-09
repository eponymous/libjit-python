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

PyObject *
convert_c2py__jit_type_t(jit_type_t cvalue, int copy)
{
    PyJit_type *py_jit_type;

    if (cvalue == NULL) {
        Py_RETURN_NONE;
    }

    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = copy ? jit_type_copy(cvalue) : cvalue;

    return (PyObject *)py_jit_type;
}

static PyObject *
py_jit_type_get_abi(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_get_abi(self->obj));
}

static PyObject *
py_jit_type_get_kind(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_get_kind(self->obj));
}

static PyObject *
py_jit_type_get_size(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_get_size(self->obj));
}

static PyObject *
py_jit_type_get_alignment(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_get_alignment(self->obj));
}

static PyObject *
py_jit_type_num_fields(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_num_fields(self->obj));
}

static PyObject *
py_jit_type_get_return(PyJit_type *self, void * UNUSED(closure))
{
    return convert_c2py__jit_type_t(jit_type_get_return(self->obj), 1);
}

static PyObject *
py_jit_type_get_tagged_type(PyJit_type *self, void * UNUSED(closure))
{
    return convert_c2py__jit_type_t(jit_type_get_tagged_type(self->obj), 1);
}

static int
py_jit_type_set_tagged_type(PyJit_type *self, PyObject *value, void * UNUSED(closure))
{
    if (PyObject_IsInstance(value, (PyObject *)&PyJit_type_Type) == 0) {
        return -1;
    }

    jit_type_set_tagged_type(self->obj, ((PyJit_type *)value)->obj, 1);

    return 0;
}

static PyObject *
py_jit_type_get_tagged_kind(PyJit_type *self, void * UNUSED(closure))
{
    return Py_BuildValue((char *) "i", jit_type_get_tagged_kind(self->obj));
}

static PyObject *
py_jit_type_get_tagged_data(PyJit_type *self, void * UNUSED(closure))
{
    PyObject *py_retval = (PyObject *)jit_type_get_tagged_data(self->obj);

    if (py_retval == NULL) {
        Py_RETURN_NONE;
    }

    return Py_BuildValue((char *) "O", py_retval);
}

static int
py_jit_type_set_tagged_data(PyJit_type *self, PyObject *value, void * UNUSED(closure))
{
    Py_XINCREF(value);

    jit_type_set_tagged_data(self->obj, value, py_jit_meta_free_func);

    return 0;
}

static PyObject *
py_jit_type_is_primitive(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_primitive(self->obj));
}

static PyObject *
py_jit_type_is_struct(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_struct(self->obj));
}

static PyObject *
py_jit_type_is_union(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_union(self->obj));
}

static PyObject *
py_jit_type_is_signature(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_signature(self->obj));
}

static PyObject *
py_jit_type_is_pointer(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_pointer(self->obj));
}

static PyObject *
py_jit_type_is_tagged(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_is_tagged(self->obj));
}

static PyObject *
py_jit_type_return_via_pointer(PyJit_type *self, void * UNUSED(closure))
{
    return PyBool_FromLong(jit_type_return_via_pointer(self->obj));
}

static PyGetSetDef PyJit_type__getsets[] = {
    {
        (char*) "abi",                                            /* attribute name */
        (getter) py_jit_type_get_abi,                             /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "kind",                                           /* attribute name */
        (getter) py_jit_type_get_kind,                            /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "size",                                           /* attribute name */
        (getter) py_jit_type_get_size,                            /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "alignment",                                      /* attribute name */
        (getter) py_jit_type_get_alignment,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "len_fields",                                     /* attribute name */
        (getter) py_jit_type_num_fields,                          /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "return_value",                                   /* attribute name */
        (getter) py_jit_type_get_return,                          /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "tagged_type",                                    /* attribute name */
        (getter) py_jit_type_get_tagged_type,                     /* C function to get the attribute */
        (setter) py_jit_type_set_tagged_type,                     /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "tagged_kind",                                    /* attribute name */
        (getter) py_jit_type_get_tagged_kind,                     /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "tagged_data",                                    /* attribute name */
        (getter) py_jit_type_get_tagged_data,                     /* C function to get the attribute */
        (setter) py_jit_type_set_tagged_data,                     /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_primitive",                                   /* attribute name */
        (getter) py_jit_type_is_primitive,                        /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_struct",                                      /* attribute name */
        (getter) py_jit_type_is_struct,                           /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_union",                                       /* attribute name */
        (getter) py_jit_type_is_union,                            /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_signature",                                   /* attribute name */
        (getter) py_jit_type_is_signature,                        /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_pointer",                                     /* attribute name */
        (getter) py_jit_type_is_pointer,                          /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_tagged",                                      /* attribute name */
        (getter) py_jit_type_is_tagged,                           /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "return_via_pointer",                             /* attribute name */
        (getter) py_jit_type_return_via_pointer,                  /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static PyObject*
py_jit_type_copy(PyJit_type *self)
{
    return convert_c2py__jit_type_t(self->obj, 1);
}

static PyObject *
py_jit_type_get_name(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    char const *retval;
    unsigned int field_index;
    const char *keywords[] = {"index", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &field_index)) {
        return NULL;
    }

    retval = jit_type_get_name(self->obj, field_index);

    return Py_BuildValue((char *) "s", retval);
}

static PyObject *
py_jit_type_find_name(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    int retval;
    char const *name;
    const char *keywords[] = {"name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &name)) {
        return NULL;
    }

    retval = jit_type_find_name(self->obj, name);

    return Py_BuildValue((char *) "i", retval);
}

static PyObject *
py_jit_type_set_names(PyJit_type *self, PyObject *args)
{
    int num_names = PySequence_Size(args);
    char *names_ptr[num_names];

    for (int i = 0; i < num_names; i++) {
        PyObject *tmp = PySequence_GetItem(args, i);

        if (PyString_Check(tmp) == 0) {
            PyErr_SetString(PyExc_TypeError, "Type.set_fields() argument 'names' must be a sequence of strings.");

            Py_DECREF(tmp);

            return NULL;
        }
#if PY_VERSION_HEX >= 0x03000000
        char *cstr;
        Py_ssize_t len;

        PyObject *str = PyUnicode_AsUTF8String(tmp);

        PyBytes_AsStringAndSize(str, &cstr, &len);

        names_ptr[i] = (char *) malloc(len+1);
        memcpy(names_ptr[i], cstr, len+1);

        Py_DECREF(str);
#else
        names_ptr[i] = PyString_AsString(tmp);
#endif
        Py_DECREF(tmp);
    }

    int retval = jit_type_set_names(self->obj, names_ptr, num_names);

#if PY_VERSION_HEX >= 0x03000000
    for (int i = 0; i < num_names; i++) {
        free(names_ptr[i]);
    }
#endif

    if (retval == 0)
        return PyErr_NoMemory();

    Py_RETURN_NONE;
}

static PyObject *
py_jit_type_set_size_and_alignment(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    int size, alignment;
    const char *keywords[] = {"size", "alignment", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "ii", (char **) keywords, &size, &alignment)) {
        return NULL;
    }

    jit_type_set_size_and_alignment(self->obj, size, alignment);

    Py_RETURN_NONE;
}

static PyObject *
py_jit_type_get_offset(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    unsigned long retval;
    unsigned int field_index;
    const char *keywords[] = {"index", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &field_index)) {
        return NULL;
    }

    retval = jit_type_get_offset(self->obj, field_index);

    return Py_BuildValue((char *) "k", retval);
}

static PyObject *
py_jit_type_set_offset(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    unsigned int field_index;
    unsigned long offset;
    const char *keywords[] = {"index", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "Ik", (char **) keywords, &field_index, &offset)) {
        return NULL;
    }

    jit_type_set_offset(self->obj, field_index, offset);

    Py_RETURN_NONE;
}

static PyObject *
py_jit_type_get_field(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    unsigned int field_index;
    const char *keywords[] = {"index", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &field_index)) {
        return NULL;
    }

    return convert_c2py__jit_type_t(jit_type_get_field(self->obj, field_index), 1);
}

static PyObject *
py_jit_type_get_param(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    unsigned int param_index;
    const char *keywords[] = {"index", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &param_index)) {
        return NULL;
    }

    return convert_c2py__jit_type_t(jit_type_get_param(self->obj, param_index), 1);
}

static PyObject *
py_jit_type_normalize(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    const char *keywords[] = {NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return NULL;
    }

    return convert_c2py__jit_type_t(jit_type_normalize(self->obj), 1);
}

static PyObject *
py_jit_remove_tags(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    const char *keywords[] = {NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return NULL;
    }

    return convert_c2py__jit_type_t(jit_type_remove_tags(self->obj), 1);
}

static PyObject *
py_jit_type_has_tag(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    int kind;
    const char *keywords[] = {"kind", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &kind)) {
        return NULL;
    }

    return PyBool_FromLong(jit_type_has_tag(self->obj, kind));
}

static PyMethodDef PyJit_type_methods[] = {
    {(char *) "__copy__",               (PyCFunction) py_jit_type_copy,                   METH_NOARGS,                NULL },
    {(char *) "get_name",               (PyCFunction) py_jit_type_get_name,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "find_name",              (PyCFunction) py_jit_type_find_name,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "set_names",              (PyCFunction) py_jit_type_set_names,              METH_VARARGS,               NULL },
    {(char *) "set_size_and_alignment", (PyCFunction) py_jit_type_set_size_and_alignment, METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "get_offset",             (PyCFunction) py_jit_type_get_offset,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "set_offset",             (PyCFunction) py_jit_type_set_offset,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "get_field",              (PyCFunction) py_jit_type_get_field,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "get_parameter",          (PyCFunction) py_jit_type_get_param,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "normalize",              (PyCFunction) py_jit_type_normalize,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "remove_tags",            (PyCFunction) py_jit_remove_tags,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "has_tag",                (PyCFunction) py_jit_type_has_tag,                METH_KEYWORDS|METH_VARARGS, NULL },
    {NULL, NULL, 0, NULL}
};

static int
PyJit_type__tp_init(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyJit_type *other;
    const char *keywords[] = {"other", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &other)) {
        return -1;
    }

    self->obj = jit_type_copy(other->obj);

    return 0;
}

static void
PyJit_type__tp_dealloc(PyJit_type *self)
{
    if (self->obj) {
        jit_type_t tmp = self->obj;
        self->obj = NULL;
        jit_type_free(tmp);
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static Py_ssize_t
py_jit_type_mp_length(PyObject *self)
{
    unsigned int type = jit_type_get_kind(((PyJit_type *)self)->obj);

    if (type == JIT_TYPE_SIGNATURE) {
        return (Py_ssize_t)jit_type_num_params(((PyJit_type *)self)->obj);
    } else if (type == JIT_TYPE_STRUCT || type == JIT_TYPE_UNION) {
        return (Py_ssize_t)jit_type_num_fields(((PyJit_type *)self)->obj);
    }

    return 0;
}

static PyObject *
py_jit_type_mp_subscript(PyObject *self, PyObject *key)
{
    jit_type_t retval = NULL;
    unsigned int index = JIT_INVALID_NAME;

    if (PyString_Check(key)) {
#if PY_VERSION_HEX >= 0x03000000
        PyObject *str = PyUnicode_AsUTF8String(key);

        index = jit_type_find_name(((PyJit_type *)self)->obj, PyBytes_AsString(str));

        Py_XDECREF(str);
#else
        index = jit_type_find_name(((PyJit_type *)self)->obj, PyString_AsString(key));
#endif
    }

    if (index != JIT_INVALID_NAME) {
        switch (jit_type_get_kind(((PyJit_type *)self)->obj)) {
            case JIT_TYPE_SIGNATURE:
                retval = jit_type_get_param(((PyJit_type *)self)->obj, index);
                break;
            case JIT_TYPE_STRUCT:
            case JIT_TYPE_UNION:
                retval = jit_type_get_field(((PyJit_type *)self)->obj, index);
                break;
            default:
                break;
        }
    }

    if (retval == NULL) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    return convert_c2py__jit_type_t(retval, 1);
}

static PyMappingMethods PyJit_type_as_mapping = {
    py_jit_type_mp_length,                                        /* mp_length */
    py_jit_type_mp_subscript,                                     /* mp_subscript */
    NULL,                                                         /* mp_ass_subscript */
};

static PyObject *
PyJit_type_tp_richcompare (PyJit_type *self, PyJit_type *other, int opid)
{
    if (PyObject_IsInstance((PyObject*) other, (PyObject*) &PyJit_type_Type)) {
        switch (opid) {
            case Py_EQ:
                if (self->obj == other->obj) {
                    Py_INCREF(Py_True);
                    return Py_True;
                } else {
                    Py_INCREF(Py_False);
                    return Py_False;
                }
            case Py_NE:
                if (self->obj != other->obj) {
                    Py_INCREF(Py_True);
                    return Py_True;
                } else {
                    Py_INCREF(Py_False);
                    return Py_False;
                }
            default:
                break;
        }
    }

    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

PyTypeObject PyJit_type_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.type.Type",                                     /* tp_name */
    sizeof(PyJit_type),                                           /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_type__tp_dealloc,                           /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)NULL,                                     /* tp_as_sequence */
    (PyMappingMethods*)&PyJit_type_as_mapping,                    /* tp_as_mapping */
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
    (richcmpfunc)PyJit_type_tp_richcompare,                       /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)NULL,                                            /* tp_iter */
    (iternextfunc)NULL,                                           /* tp_iternext */
    (struct PyMethodDef*)PyJit_type_methods,                      /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_type__getsets,                                          /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_type__tp_init,                                /* tp_init */
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
py_jit_type_create_struct(PyJit_type *UNUSED(dummy), PyObject *args)
{
    int num_fields = PySequence_Size(args);
    jit_type_t fields_ptr[num_fields];

    for (int i = 0; i < num_fields; i++) {
        PyObject *tmp = PySequence_GetItem(args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_type_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Type.create_struct() arguments must be of 'jit.type.Type'.");
            Py_DECREF(tmp);
            return NULL;
        }

        fields_ptr[i] = ((PyJit_type*)tmp)->obj;
        Py_DECREF(tmp);
    }

    jit_type_t retval = jit_type_create_struct(fields_ptr, num_fields, 1);

    return convert_c2py__jit_type_t(retval, 0);
}

static PyObject *
py_jit_type_create_union(PyJit_type *UNUSED(dummy), PyObject *args)
{
    int num_fields = PySequence_Size(args);
    jit_type_t fields_ptr[num_fields];

    for (int i = 0; i < num_fields; i++) {
        PyObject *tmp = PySequence_GetItem(args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_type_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Type.create_union() arguments must be of 'jit.type.Type'.");
            Py_DECREF(tmp);
            return NULL;
        }

        fields_ptr[i] = ((PyJit_type*)tmp)->obj;
        Py_DECREF(tmp);
    }

    jit_type_t retval = jit_type_create_union(fields_ptr, num_fields, 1);

    return convert_c2py__jit_type_t(retval, 0);
}

static PyObject *
py_jit_type_create_signature(PyJit_type *UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    jit_abi_t abi;
    PyJit_type *return_type;
    PyObject *params;
    const char *keywords[] = {"abi", "return_type", "params", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "iO!O", (char **) keywords, &abi, &PyJit_type_Type, &return_type, &params)) {
        return NULL;
    }

    if (PySequence_Check(params) == 0) {
        PyErr_SetString(PyExc_TypeError, "Type.create_signature() argument 'params' must be a sequence.");
        return NULL;
    }

    int num_params = PySequence_Size(params);
    jit_type_t params_ptr[num_params];

    for (int i = 0; i < num_params; i++) {
        PyObject *tmp = PySequence_GetItem(params, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_type_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Type.create_signature() argument 'params' must be a sequence of 'jit.Type'.");
            Py_DECREF(tmp);
            return NULL;
        }

        params_ptr[i] = ((PyJit_type*)tmp)->obj;
        Py_DECREF(tmp);
    }

    jit_type_t retval = jit_type_create_signature(abi, return_type->obj, params_ptr, num_params, 1);

    return convert_c2py__jit_type_t(retval, 0);
}

static PyObject *
py_jit_type_create_pointer(PyJit_type *UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyJit_type *type;
    const char *keywords[] = {"type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &type)) {
        return NULL;
    }

    return convert_c2py__jit_type_t(jit_type_create_pointer(type->obj, 1), 0);
}

static PyObject *
py_jit_type_create_tagged(PyJit_type *UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyJit_type *type;
    int kind;
    PyObject *data = NULL;
    const char *keywords[] = {"type", "kind", "data", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!i|O", (char **) keywords, &PyJit_type_Type, &type, &kind, &data)) {
        return NULL;
    }

    Py_XINCREF(data);

    return convert_c2py__jit_type_t(jit_type_create_tagged(type->obj, kind, data, py_jit_meta_free_func, 1), 0);
}

static PyMethodDef jit_type_functions[] = {
    {(char *) "create_struct",          (PyCFunction) py_jit_type_create_struct,          METH_VARARGS,               NULL },
    {(char *) "create_signature",       (PyCFunction) py_jit_type_create_signature,       METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "create_union",           (PyCFunction) py_jit_type_create_union,           METH_VARARGS,               NULL },
    {(char *) "create_pointer",         (PyCFunction) py_jit_type_create_pointer,         METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "create_tagged",          (PyCFunction) py_jit_type_create_tagged,          METH_KEYWORDS|METH_VARARGS, NULL },
    {NULL, NULL, 0, NULL}
};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_type_sys_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.types.sys",
    NULL,
    -1,
    NULL,
};
#endif

static PyObject *
initjit_type_sys(void)
{
    PyObject *m;

    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_type_sys_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.types.sys", NULL, NULL);
    #endif

    if (m == NULL) {
        return NULL;
    }

    PyModule_AddObject(m, (char *) "bool",        convert_c2py__jit_type_t(jit_type_sys_bool, 1));
    PyModule_AddObject(m, (char *) "char",        convert_c2py__jit_type_t(jit_type_sys_char, 1));
    PyModule_AddObject(m, (char *) "schar",       convert_c2py__jit_type_t(jit_type_sys_schar, 1));
    PyModule_AddObject(m, (char *) "uchar",       convert_c2py__jit_type_t(jit_type_sys_short, 1));
    PyModule_AddObject(m, (char *) "short",       convert_c2py__jit_type_t(jit_type_sys_short, 1));
    PyModule_AddObject(m, (char *) "ushort",      convert_c2py__jit_type_t(jit_type_sys_ushort, 1));
    PyModule_AddObject(m, (char *) "int",         convert_c2py__jit_type_t(jit_type_sys_int, 1));
    PyModule_AddObject(m, (char *) "uint",        convert_c2py__jit_type_t(jit_type_sys_uint, 1));
    PyModule_AddObject(m, (char *) "long",        convert_c2py__jit_type_t(jit_type_sys_long, 1));
    PyModule_AddObject(m, (char *) "ulong",       convert_c2py__jit_type_t(jit_type_sys_ulong, 1));
    PyModule_AddObject(m, (char *) "longlong",    convert_c2py__jit_type_t(jit_type_sys_longlong, 1));
    PyModule_AddObject(m, (char *) "ulonglong",   convert_c2py__jit_type_t(jit_type_sys_ulonglong, 1));
    PyModule_AddObject(m, (char *) "float",       convert_c2py__jit_type_t(jit_type_sys_float, 1));
    PyModule_AddObject(m, (char *) "double",      convert_c2py__jit_type_t(jit_type_sys_double, 1));
    PyModule_AddObject(m, (char *) "long_double", convert_c2py__jit_type_t(jit_type_sys_long_double, 1));

    return m;
}

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_type_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.type",
    NULL,
    -1,
    jit_type_functions,
};
#endif

PyObject *
initjit_type(void)
{
    PyObject *m, *submodule;

    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_type_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.types", jit_type_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }

    submodule = initjit_type_sys();

    if (submodule == NULL) {
        return NULL;
    }
    Py_INCREF(submodule);

    PyModule_AddObject(m, (char *) "sys", submodule);

    /* Register the 'jit_type_t' class */
    if (PyType_Ready(&PyJit_type_Type)) {
        return NULL;
    }
    PyModule_AddObject(m, (char *) "Type", (PyObject *) &PyJit_type_Type);

    PyModule_AddObject(m, (char *) "void",     convert_c2py__jit_type_t(jit_type_void, 1));
    PyModule_AddObject(m, (char *) "sbyte",    convert_c2py__jit_type_t(jit_type_sbyte, 1));
    PyModule_AddObject(m, (char *) "ubyte",    convert_c2py__jit_type_t(jit_type_ubyte, 1));
    PyModule_AddObject(m, (char *) "short",    convert_c2py__jit_type_t(jit_type_short, 1));
    PyModule_AddObject(m, (char *) "ushort",   convert_c2py__jit_type_t(jit_type_ushort, 1));
    PyModule_AddObject(m, (char *) "int",      convert_c2py__jit_type_t(jit_type_int, 1));
    PyModule_AddObject(m, (char *) "uint",     convert_c2py__jit_type_t(jit_type_uint, 1));
    PyModule_AddObject(m, (char *) "nint",     convert_c2py__jit_type_t(jit_type_nint, 1));
    PyModule_AddObject(m, (char *) "nuint",    convert_c2py__jit_type_t(jit_type_nuint, 1));
    PyModule_AddObject(m, (char *) "long",     convert_c2py__jit_type_t(jit_type_long, 1));
    PyModule_AddObject(m, (char *) "ulong",    convert_c2py__jit_type_t(jit_type_ulong, 1));
    PyModule_AddObject(m, (char *) "float32",  convert_c2py__jit_type_t(jit_type_float32, 1));
    PyModule_AddObject(m, (char *) "float64",  convert_c2py__jit_type_t(jit_type_float64, 1));
    PyModule_AddObject(m, (char *) "nfloat",   convert_c2py__jit_type_t(jit_type_nfloat, 1));
    PyModule_AddObject(m, (char *) "void_ptr", convert_c2py__jit_type_t(jit_type_void_ptr, 1));

    return m;
}

