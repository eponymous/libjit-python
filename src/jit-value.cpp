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
convert_c2py__jit_value(jit_value *cvalue)
{
    PyJit_value *py_jit_value;

    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->obj = new jit_value(*cvalue);

    return (PyObject *)py_jit_value;
}


int
convert_py2c__jit_value(PyObject *value, jit_value *address)
{
    if (PyObject_IsInstance(value, (PyObject *)&PyJit_value_Type)) {
        *address = *((PyJit_value *)value)->obj;
        return 1;
    }

    return 0;
}

static PyObject *
PyJit_value__get_is_addressable(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_addressable());
}

static PyObject *
PyJit_value__get_is_constant(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_constant());
}

static PyObject *
PyJit_value__get_is_parameter(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_parameter());
}

static PyObject *
PyJit_value__get_is_temporary(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_temporary());
}

static PyObject *
PyJit_value__get_is_local(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_local());
}

static PyObject *
PyJit_value__get_is_valid(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_valid());
}

static PyObject *
PyJit_value__get_is_volatile(PyJit_value *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_volatile());
}

static PyObject *
PyJit_value__get_block(PyJit_value *self, void * UNUSED(closure))
{
    jit_block_t retval = self->obj->block();

    if (retval == NULL) {
        Py_RETURN_NONE;
    }

    PyJit_block *py_jit_block = PyObject_New(PyJit_block, &PyJit_block_Type);
    py_jit_block->obj = retval;

    return (PyObject *) py_jit_block;
}

static PyObject *
PyJit_value__get_constant(PyJit_value *self, void * UNUSED(closure))
{
    jit_constant_t retval = self->obj->constant();

    switch (jit_type_get_kind(retval.type)) {
        case JIT_TYPE_SBYTE:
        case JIT_TYPE_UBYTE:
        case JIT_TYPE_SHORT:
        case JIT_TYPE_USHORT:
        case JIT_TYPE_INT:
        case JIT_TYPE_UINT:
             return PyInt_FromLong(retval.un.long_value);
        case JIT_TYPE_NINT:
        case JIT_TYPE_NUINT:
        case JIT_TYPE_LONG:
        case JIT_TYPE_ULONG:
             return PyLong_FromLong(retval.un.long_value);
        case JIT_TYPE_FLOAT32:
        case JIT_TYPE_FLOAT64:
        case JIT_TYPE_NFLOAT:
            return PyFloat_FromDouble(retval.un.float64_value);
        default:
            break;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_value__get_context(PyJit_value *self, void * UNUSED(closure))
{
    PyJit_context *py_jit_context;
    jit_context_t retval = self->obj->context();

    if (retval == NULL) {
        Py_RETURN_NONE;
    }

    py_jit_context = PyObject_New(PyJit_context, &PyJit_context_Type);
    py_jit_context->obj = new jit_context(retval);

    return (PyObject *)py_jit_context;
}

static PyObject *
PyJit_value__get_function(PyJit_value *self, void * UNUSED(closure))
{
    return convert_c2py__jit_function(jit_function::from_raw(self->obj->function()), 1);
}

static PyObject *
PyJit_value__get_type(PyJit_value *self, void * UNUSED(closure))
{
    return convert_c2py__jit_type_t(self->obj->type(), 1);
}

static PyGetSetDef PyJit_value__getsets[] = {
    {
        (char*) "is_addressable",                                 /* attribute name */
        (getter) PyJit_value__get_is_addressable,                 /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_constant",                                    /* attribute name */
        (getter) PyJit_value__get_is_constant,                    /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_local",                                       /* attribute name */
        (getter) PyJit_value__get_is_local,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_parameter",                                   /* attribute name */
        (getter) PyJit_value__get_is_parameter,                   /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_temporary",                                   /* attribute name */
        (getter) PyJit_value__get_is_temporary,                   /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_valid",                                       /* attribute name */
        (getter) PyJit_value__get_is_valid,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_volatile",                                    /* attribute name */
        (getter) PyJit_value__get_is_volatile,                    /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "block",                                          /* attribute name */
        (getter) PyJit_value__get_block,                          /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "constant",                                       /* attribute name */
        (getter) PyJit_value__get_constant,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "context",                                        /* attribute name */
        (getter) PyJit_value__get_context,                        /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "function",                                       /* attribute name */
        (getter) PyJit_value__get_function,                       /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "type",                                           /* attribute name */
        (getter) PyJit_value__get_type,                           /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static PyObject *
PyJit_value__copy__(PyJit_value *self)
{
    PyJit_value *py_copy = PyObject_New(PyJit_value, &PyJit_value_Type);

    py_copy->obj = new jit_value(*self->obj);

    return (PyObject *) py_copy;
}

static PyObject *
PyJit_value_set_addressable(PyJit_value *self)
{
    self->obj->set_addressable();

    Py_RETURN_NONE;
}

static PyObject *
PyJit_value_set_volatile(PyJit_value *self)
{
    self->obj->set_volatile();

    Py_RETURN_NONE;
}

static PyMethodDef PyJit_value_methods[] = {
    {(char *) "__copy__",           (PyCFunction) PyJit_value__copy__,            METH_NOARGS, NULL},
    {(char *) "set_addressable",    (PyCFunction) PyJit_value_set_addressable,    METH_NOARGS, NULL },
    {(char *) "set_volatile",       (PyCFunction) PyJit_value_set_volatile,       METH_NOARGS, NULL },
    {NULL, NULL, 0, NULL}
};

typedef jit_value_t (*jit_insn_binop)(jit_function_t, jit_value_t, jit_value_t);
typedef jit_value_t (*jit_insn_uniop)(jit_function_t, jit_value_t);

template<jit_insn_binop op>
static PyObject *
Jit_value__nb_binop(PyJit_value *left, PyJit_value *right)
{
    jit_function_t func = jit_value_get_function(left->obj->raw());
    func = (func ? func : jit_value_get_function(right->obj->raw()));

    jit_value result(op(func, left->obj->raw(), right->obj->raw()));

    return convert_c2py__jit_value(&result);
}

template<jit_insn_uniop op>
static PyObject *
Jit_value__nb_uniop(PyJit_value *self)
{
    jit_value result(op(jit_value_get_function(self->obj->raw()), self->obj->raw()));

    return convert_c2py__jit_value(&result);
}


#if PY_VERSION_HEX < 0x03000000
static PyNumberMethods Jit_value__py_number_methods = {
    (binaryfunc)  Jit_value__nb_binop<jit_insn_add>,              /* nb_add */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_sub>,              /* nb_subtract */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_mul>,              /* nb_multiply */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_div>,              /* nb_divide */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_rem>,              /* nb_remainder */
    (binaryfunc)  NULL,                                           /* nb_divmod */
    (ternaryfunc) Jit_value__nb_binop<jit_insn_pow>,              /* nb_power */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_neg>,              /* nb_negative */
    (unaryfunc)   NULL,                                           /* nb_positive */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_abs>,              /* nb_absolute */
    (inquiry)     NULL,                                           /* nb_nonzero */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_not>,              /* nb_invert */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_shl>,              /* nb_lshift */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_shr>,              /* nb_rshift */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_and>,              /* nb_and */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_xor>,              /* nb_xor */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_or>,               /* nb_or */
    (coercion)    NULL,                                           /* nb_coerce */
    (unaryfunc)   NULL,                                           /* nb_int */
    (unaryfunc)   NULL,                                           /* nb_long */
    (unaryfunc)   NULL,                                           /* nb_float */
    (unaryfunc)   NULL,                                           /* nb_oct */
    (unaryfunc)   NULL,                                           /* nb_hex */
    (binaryfunc)  NULL,                                           /* nb_inplace_add */
    (binaryfunc)  NULL,                                           /* nb_inplace_subtract */
    (binaryfunc)  NULL,                                           /* nb_inplace_multiply */
    (binaryfunc)  NULL,                                           /* nb_inplace_divide */
    (binaryfunc)  NULL,                                           /* nb_inplace_remainder */
    (ternaryfunc) NULL,                                           /* nb_inplace_power */
    (binaryfunc)  NULL,                                           /* nb_inplace_lshift */
    (binaryfunc)  NULL,                                           /* nb_inplace_rshift */
    (binaryfunc)  NULL,                                           /* nb_inplace_and */
    (binaryfunc)  NULL,                                           /* nb_inplace_xor */
    (binaryfunc)  NULL,                                           /* nb_inplace_or */
    (binaryfunc) NULL,                                            /* nb_floor_divide */
    (binaryfunc) NULL,                                            /* nb_true_divide */
    (binaryfunc) NULL,                                            /* nb_inplace_floor_divide */
    (binaryfunc) NULL,                                            /* nb_inplace_true_divide */
    (unaryfunc)  NULL,    /*  */
};
#else /* Py3k */
static PyNumberMethods Jit_value__py_number_methods = {
    (binaryfunc)  Jit_value__nb_binop<jit_insn_add>,              /* nb_add */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_sub>,              /* nb_subtract */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_mul>,              /* nb_multiply */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_rem>,              /* nb_remainder */
    (binaryfunc)  NULL,                                           /* nb_divmod */
    (ternaryfunc) Jit_value__nb_binop<jit_insn_pow>,              /* nb_power */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_neg>,              /* nb_negative */
    (unaryfunc)   NULL,                                           /* nb_positive */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_abs>,              /* nb_absolute */
    (inquiry)     NULL,                                           /* nb_bool */
    (unaryfunc)   Jit_value__nb_uniop<jit_insn_not>,              /* nb_invert */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_shl>,              /* nb_lshift */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_shl>,              /* nb_rshift */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_and>,              /* nb_and */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_xor>,              /* nb_xor */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_or>,               /* nb_or */
    (unaryfunc)   NULL,                                           /* nb_int */
    NULL,                                                         /* nb_reserved */
    (unaryfunc)   NULL,                                           /* nb_float */
    (binaryfunc)  NULL,                                           /* nb_inplace_add */
    (binaryfunc)  NULL,                                           /* nb_inplace_subtract */
    (binaryfunc)  NULL,                                           /* nb_inplace_multiply */
    (binaryfunc)  NULL,                                           /* nb_inplace_remainder */
    (ternaryfunc) NULL,                                           /* nb_inplace_power */
    (binaryfunc)  NULL,                                           /* nb_inplace_lshift */
    (binaryfunc)  NULL,                                           /* nb_inplace_rshift */
    (binaryfunc)  NULL,                                           /* nb_inplace_and */
    (binaryfunc)  NULL,                                           /* nb_inplace_xor */
    (binaryfunc)  NULL,                                           /* nb_inplace_or */
    (binaryfunc)  NULL,                                           /* nb_floor_divide */
    (binaryfunc)  Jit_value__nb_binop<jit_insn_div>,              /* nb_divide */
    (binaryfunc)  NULL,                                           /* nb_inplace_floor_divide */
    (binaryfunc)  NULL,                                           /* nb_inplace_divide */
    (unaryfunc)   NULL,                                           /* nb_index */
};
#endif /* PY_VERSION_HEX < 0x03000000 */

static PyObject *
PyJit_value__tp_richcompare (PyJit_value *left, PyJit_value *right, int opid)
{
    jit_value result;
    jit_function_t func = jit_value_get_function(left->obj->raw());
    func = (func ? func : jit_value_get_function(right->obj->raw()));

    switch (opid) {
        case Py_LT:
            result = jit_insn_lt(func, left->obj->raw(), right->obj->raw());
            break;
        case Py_LE:
            result = jit_insn_le(func, left->obj->raw(), right->obj->raw());
            break;
        case Py_EQ:
            result = jit_insn_eq(func, left->obj->raw(), right->obj->raw());
            break;
        case Py_NE:
            result = jit_insn_ne(func, left->obj->raw(), right->obj->raw());
            break;
        case Py_GE:
            result = jit_insn_ge(func, left->obj->raw(), right->obj->raw());
            break;
        case Py_GT:
            result = jit_insn_gt(func, left->obj->raw(), right->obj->raw());
            break;
    }

    return convert_c2py__jit_value(&result);
}

static int
PyJit_value__tp_init(PyJit_value *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *copy = NULL;
    const char *keywords[] = {"copy", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "|O!", (char **) keywords, &PyJit_value_Type, &copy)) {
        return -1;
    }

    if (copy) {
        self->obj = new jit_value(*((PyJit_value *) copy)->obj);
    } else {
        self->obj = new jit_value();
    }

    return 0;
}

static void
PyJit_value__tp_dealloc(PyJit_value *self)
{
    delete self->obj;
    self->obj = NULL;

    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_value_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Value",                                         /* tp_name */
    sizeof(PyJit_value),                                          /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_value__tp_dealloc,                          /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)&Jit_value__py_number_methods,              /* tp_as_number */
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
    (richcmpfunc)PyJit_value__tp_richcompare,                     /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)NULL,                                            /* tp_iter */
    (iternextfunc)NULL,                                           /* tp_iternext */
    (struct PyMethodDef*)PyJit_value_methods,                     /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_value__getsets,                                         /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    0,                                                            /* tp_dictoffset */
    (initproc)PyJit_value__tp_init,                               /* tp_init */
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

