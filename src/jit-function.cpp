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
#include <jit/jit-dump.h>
#include <typeinfo>
#include <stdio.h>

void
PyJit_function__PythonHelper::build()
{
    PyGILState_STATE __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);

    PyObject *py_method = PyObject_GetAttrString(m_pyself, (char *) "build");

    PyErr_Clear();

    if (py_method == NULL || PyCFunction_Check(py_method)) {
        Py_XDECREF(py_method);

        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);

        jit_function::build();

        return;
    }

    jit_function *self_obj_before = reinterpret_cast< PyJit_function* >(m_pyself)->obj;
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = (jit_function *) this;

    if (PyObject_CallMethod(m_pyself, (char *) "build", (char *) "") == NULL) {
        PyErr_Print();
    }

    reinterpret_cast< PyJit_function * >(m_pyself)->obj = self_obj_before;

    Py_XDECREF(py_method);

    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);

    return;
}

jit_type_t
PyJit_function__PythonHelper::create_signature()
{
    PyGILState_STATE __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);

    PyObject *py_method = PyObject_GetAttrString(m_pyself, (char *) "create_signature");

    PyErr_Clear();

    if (py_method == NULL || PyCFunction_Check(py_method)) {
        Py_XDECREF(py_method);

        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);

        return jit_function::create_signature();
    }

    Py_XDECREF(py_method);

    jit_function *self_obj_before = reinterpret_cast< PyJit_function * >(m_pyself)->obj;
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = (jit_function*) this;

    PyObject *py_retval = PyObject_CallMethod(m_pyself, (char *) "create_signature", (char *) "");

    if (py_retval == NULL || !PyObject_IsInstance(py_retval, (PyObject *)&PyJit_type_Type)) {
        if (py_retval)
            PyErr_SetString(PyExc_TypeError, "Function.create_signature() should return a 'jit.Type' type");
        else
            PyErr_Print();

        reinterpret_cast< PyJit_function * >(m_pyself)->obj = self_obj_before;

        Py_XDECREF(py_retval);

        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);

        return NULL;
    }

    jit_type_t retval = jit_type_copy(((PyJit_type *)py_retval)->obj);

    reinterpret_cast< PyJit_function * >(m_pyself)->obj = self_obj_before;

    Py_DECREF(py_retval);

    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);

    return retval;
}

PyObject *
convert_c2py__jit_function(jit_function *cvalue, int copy)
{
    PyJit_function *py_jit_function;

    if (cvalue == NULL) {
        Py_RETURN_NONE;
    }

    if (typeid(*cvalue).name() == typeid(PyJit_function__PythonHelper).name())
    {
        py_jit_function = reinterpret_cast< PyJit_function * >(reinterpret_cast< PyJit_function__PythonHelper * >(cvalue)->m_pyself);
        // py_jit_function->obj = cvalue;
        Py_INCREF(py_jit_function);
    } else {
        py_jit_function = PyObject_GC_New(PyJit_function, &PyJit_function_Type);
        py_jit_function->inst_dict = NULL;
        py_jit_function->obj = new jit_function(*cvalue);
        py_jit_function->flags = copy ? JIT_FLAG_OBJECT_NOT_OWNED : JIT_FLAG_NONE;
    }

    return (PyObject *) py_jit_function;
}

static PyObject *
PyJit_function__get_blocks(PyJit_function *self, void * UNUSED(closure))
{
    PyJit_block_list *iter = PyObject_New(PyJit_block_list, &PyJit_block_list_Type);

    Py_INCREF(self);
    iter->func = self;
    iter->prev = NULL;

    return (PyObject *)iter;
}

static PyObject *
PyJit_function__get_context(PyJit_function *self, void * UNUSED(closure))
{
    PyJit_context *retval;
    jit_context_t ctx = jit_function_get_context(self->obj->raw());

    retval = PyObject_New(PyJit_context, &PyJit_context_Type);
    retval->obj = new jit_context(ctx);

    return (PyObject *)retval;
}

static PyObject *
PyJit_function__get_is_recompilable(PyJit_function *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_recompilable());
}

static int
PyJit_function__set_is_recompilable(PyJit_function *self, PyObject *value, void * UNUSED(closure))
{
    self->obj->set_recompilable(PyObject_IsTrue(value));

    return 0;
}

static PyObject *
PyJit_function__get_is_compiled(PyJit_function *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_compiled());
}

static PyObject *
PyJit_function__get_is_valid(PyJit_function *self, void * UNUSED(closure))
{
    return PyBool_FromLong(self->obj->is_valid());
}

static PyObject *
PyJit_function__get_optimization_level(PyJit_function *self, void * UNUSED(closure))
{
    return PyLong_FromUnsignedLong(self->obj->optimization_level());
}

static int
PyJit_function__set_optimization_level(PyJit_function *self, PyObject *value, void * UNUSED(closure))
{
    self->obj->set_optimization_level(PyLong_AsUnsignedLong(value));

    return 0;
}

static PyObject *
PyJit_function__get_max_optimization_level(PyJit_function *self, void * UNUSED(closure))
{
    return PyLong_FromUnsignedLong(self->obj->max_optimization_level());
}

static PyObject *
PyJit_function__get_params(PyJit_function *self, void * UNUSED(closure))
{
	int num_params = jit_type_num_params(self->obj->signature());

    PyObject* py_retval = PyTuple_New(num_params);

    for (int i = 0; i < num_params; i++) {
        jit_value tmp = self->obj->get_param(i);
        PyTuple_SET_ITEM(py_retval, i, convert_c2py__jit_value(&tmp));
    }

    return py_retval;
}

static PyObject *
PyJit_function__get_signature(PyJit_function *self, void * UNUSED(closure))
{
    return convert_c2py__jit_type_t(self->obj->signature(), 1);
}

static PyGetSetDef PyJit_function__getsets[] = {
    {
        (char*) "context",                                        /* attribute name */
        (getter) PyJit_function__get_context,                     /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "blocks",                                         /* attribute name */
        (getter) PyJit_function__get_blocks,                      /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_recompilable",                                /* attribute name */
        (getter) PyJit_function__get_is_recompilable,             /* C function to get the attribute */
        (setter) PyJit_function__set_is_recompilable,             /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_compiled",                                    /* attribute name */
        (getter) PyJit_function__get_is_compiled,                 /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "is_valid",                                       /* attribute name */
        (getter) PyJit_function__get_is_valid,                    /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "optimization_level",                             /* attribute name */
        (getter) PyJit_function__get_optimization_level,          /* C function to get the attribute */
        (setter) PyJit_function__set_optimization_level,          /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "max_optimization_level",                         /* attribute name */
        (getter) PyJit_function__get_max_optimization_level,      /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "params",                                         /* attribute name */
        (getter) PyJit_function__get_params,                      /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    {
        (char*) "signature",                                      /* attribute name */
        (getter) PyJit_function__get_signature,                   /* C function to get the attribute */
        (setter) NULL,                                            /* C function to set the attribute */
        NULL,                                                     /* optional doc string */
        NULL                                                      /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

PyObject *
PyJit_function__PythonHelper::py_build(PyJit_function *self)
{
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);

    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method 'build' of class 'jit.Function' is protected and can only be called by a subclass");
        return NULL;
    }

    helper->build();

    Py_RETURN_NONE;
}

PyObject *
PyJit_function__PythonHelper::py_create_signature(PyJit_function *self)
{
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);

    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method 'create_signature' of class 'jit.Function' is protected and can only be called by a subclass");
        return NULL;
    }

    jit_type_t retval = helper->create_signature();

    return (retval ? convert_c2py__jit_type_t(retval, 0) : NULL);
}

PyObject *
PyJit_function__PythonHelper::py_fail(PyJit_function *self)
{
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);

    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method 'fail' of class 'jit.Function' is protected and can only be called by a subclass");
        return NULL;
    }

    helper->fail__parent_caller();

    Py_RETURN_NONE;
}

PyObject *
PyJit_function__PythonHelper::py_out_of_memory(PyJit_function *self)
{
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);

    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method 'out_of_memory' of class 'jit.Function' is protected and can only be called by a subclass");
        return NULL;
    }

    helper->out_of_memory__parent_caller();

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_build_start(PyJit_function *self)
{
    self->obj->build_start();

    Py_RETURN_NONE;
}


static PyObject *
PyJit_function_build_end(PyJit_function *self)
{
    self->obj->build_end();

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_compile(PyJit_function *self)
{
    return PyBool_FromLong(self->obj->compile());
}

static PyObject *
PyJit_function_create(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_type *signature = NULL;
    const char *keywords[] = {"signature", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "|O!", (char **) keywords, &PyJit_type_Type, &signature)) {
        return NULL;
    }

    if (signature && jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.create() requires a 'signature' type");
        return NULL;
    }

    if (signature) {
        self->obj->create(signature->obj);
    } else {
        self->obj->create();
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_dump(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    char *buf;
    size_t len;

    FILE *fp = open_memstream(&buf, &len);

    jit_dump_function(fp, self->obj->raw(), 0);

    fclose(fp);

    PyObject *retval = PyString_FromStringAndSize(buf, len);

    free(buf);

    return retval;
}

static PyObject *
PyJit_function_get_param(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    int param;
    const char *keywords[] = {"param", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &param)) {
        return NULL;
    }

    jit_value retval = self->obj->get_param(param);

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_get_struct_pointer(PyJit_function *self)
{
    jit_value retval = self->obj->get_struct_pointer();

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_new_constant(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_value;
    PyJit_type *type;
    jit_constant_t value;
    const char *keywords[] = {"value", "type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "OO!", (char **) keywords, &py_value, &PyJit_type_Type, &type)) {
        return NULL;
    }

    if (!PyNumber_Check(py_value)) {
        PyErr_SetString(PyExc_TypeError, "Function.new_constant() 'value' parameter must be a number.");
        return NULL;
    }

    if (self->obj->raw() == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    switch (jit_type_get_kind(type->obj)) {
        case JIT_TYPE_SBYTE:
        case JIT_TYPE_UBYTE:
        case JIT_TYPE_SHORT:
        case JIT_TYPE_USHORT:
        case JIT_TYPE_INT:
        case JIT_TYPE_UINT:
        case JIT_TYPE_NINT:
        case JIT_TYPE_NUINT:
        case JIT_TYPE_LONG:
        case JIT_TYPE_ULONG:
            value.un.long_value = PyLong_AsLong(py_value);
            break;
        case JIT_TYPE_FLOAT32:
        case JIT_TYPE_FLOAT64:
        case JIT_TYPE_NFLOAT:
            value.un.float64_value = PyFloat_AsDouble(py_value);
            break;
        default:
            PyErr_SetString(PyExc_TypeError, "Function.new_constant() 'type' could not be converted into a constant.");
            return NULL;
    }

    value.type = type->obj;

    jit_value retval = self->obj->new_constant(value);

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_new_label(PyJit_function *self)
{
    PyJit_label *py_jit_label = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_jit_label->obj = new jit_label(self->obj->new_label());

    return (PyObject *)py_jit_label;
}

static PyObject *
PyJit_function_new_value(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_type *type;
    const char *keywords[] = {"type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &type)) {
        return NULL;
    }

    try {
        retval = self->obj->new_value(type->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_label(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }

    try {
        self->obj->insn_label(*(label->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_new_block(PyJit_function *self)
{
    try {
        self->obj->insn_new_block();
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_load(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_load(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_dup(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_dup(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_load_small(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_load_small(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_store(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *dest, *value;
    const char *keywords[] = {"dest", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        self->obj->store(*((PyJit_value *) dest)->obj, *(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_load_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    unsigned int offset;
    PyJit_type *type;
    const char *keywords[] = {"value", "offset", "type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!IO!", (char **) keywords, &PyJit_value_Type, &value, &offset, &PyJit_type_Type, &type)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_load_relative(*(value->obj), offset, type->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_store_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *dest, *value;;
    unsigned int offset;
    const char *keywords[] = {"dest", "offset", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!IO!", (char **) keywords, &PyJit_value_Type, &dest, &offset, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        self->obj->insn_store_relative(*(dest->obj), offset, *(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_add_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    unsigned int offset;
    const char *keywords[] = {"value", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!I", (char **) keywords, &PyJit_value_Type, &value, &offset)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_add_relative(*(value->obj), offset);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_load_elem(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *base_addr, *index;
    PyJit_type *elem_type;
    const char *keywords[] = {"base_addr", "index", "elem_type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_type_Type, &elem_type)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_load_elem(*(base_addr->obj), *(index->obj), elem_type->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_load_elem_address(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *base_addr, *index;
    PyJit_type *elem_type;
    const char *keywords[] = {"base_addr", "index", "elem_type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_type_Type, &elem_type)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_load_elem_address(*(base_addr->obj), *(index->obj), elem_type->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_store_elem(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *base_addr, *index, *value;
    const char *keywords[] = {"base_addr", "index", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        self->obj->insn_store_elem(*(base_addr->obj), *(index->obj), *(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_check_null(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        self->obj->insn_check_null(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

typedef jit_value_t (*jit_insn_uniop)(jit_function_t, jit_value_t);
typedef jit_value_t (*jit_insn_binop)(jit_function_t, jit_value_t, jit_value_t);

template<jit_insn_uniop func>
static PyObject *
PyJit_function_insn_uniop(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    if (self->obj->raw() == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    retval = func(self->obj->raw(), value->obj->raw());

    return convert_c2py__jit_value(&retval);
}

template<jit_insn_binop func>
static PyObject *
PyJit_function_insn_binop(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *left, *right;
    const char *keywords[] = {"left", "right", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &left, &PyJit_value_Type, &right)) {
        return NULL;
    }

    if (self->obj->raw() == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    retval = func(self->obj->raw(), left->obj->raw(), right->obj->raw());

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_branch(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }

    try {
        self->obj->insn_branch(*(label->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_branch_if(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value;
    PyJit_label *label;
    const char *keywords[] = {"value", "label", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_label_Type, &label)) {
        return NULL;
    }

    try {
        self->obj->insn_branch_if(*(value->obj), *(label->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_branch_if_not(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value;
    PyJit_label *label;
    const char *keywords[] = {"value", "label", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_label_Type, &label)) {
        return NULL;
    }

    try {
        self->obj->insn_branch_if_not(*(value->obj), *(label->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_jump_table(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value;
    PyJit_jump_table *jump_table;
    const char *keywords[] = {"value", "jump_table", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_jump_table_Type, &jump_table)) {
        return NULL;
    }

    try {
        self->obj->insn_jump_table(*(value->obj), *(jump_table->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_address_of(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_address_of(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_address_of_label(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_address_of_label(*(label->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_convert(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    PyJit_type *type;
    int overflow_check = 0;
    const char *keywords[] = {"value", "type", "overflow_check", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &type, &overflow_check)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_convert(*(value->obj), type->obj, overflow_check);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_call(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    char const *name;
    PyJit_function *jit_func;
    PyJit_type *signature;
    PyObject *py_args;
    int flags = 0;
    const char *keywords[] = {"name", "jit_func", "signature", "args", "flags", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O!O|i", (char **) keywords, &name, &PyJit_function_Type, &jit_func, &PyJit_type_Type, &signature, &py_args, &flags)) {
        return NULL;
    }

    if (jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call() argument 'signature' not a signature");
        return NULL;
    }

    if (PySequence_Check(py_args) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call() argument 'args' must be a sequence.");
        return NULL;
    }

    int num_args = PySequence_Size(py_args);
    jit_value_t args_ptr[num_args];

    for (int i = 0; i < num_args; i++) {
        PyObject *tmp = PySequence_GetItem(py_args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_value_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Function.insn_call() argument 'args' must be a sequence of 'jit.Value'.");
            free(args_ptr);
            Py_DECREF(tmp);
            return NULL;
        }

        args_ptr[i] = ((PyJit_value *)tmp)->obj->raw();
        Py_DECREF(tmp);
    }

    try {
        retval = self->obj->insn_call(name, jit_func->obj->raw(), signature->obj, args_ptr, num_args, flags);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function.insn_call failed.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_call_indirect(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    PyJit_type *signature;
    PyObject *py_args;
    int flags = 0;
    const char *keywords[] = {"value", "signature", "args", "flags", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O!O|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &signature, &py_args, &flags)) {
        return NULL;
    }

    if (jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect() argument 'signature' not a signature");
        return NULL;
    }

    if (PySequence_Check(py_args) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect() argument 'args' must be a sequence.");
        return NULL;
    }

    int num_args = PySequence_Size(py_args);
    jit_value_t args_ptr[num_args];

    for (int i = 0; i < num_args; i++) {
        PyObject *tmp = PySequence_GetItem(py_args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_value_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect() argument 'args' must be a sequence of 'jit.Value'.");
            free(args_ptr);
            Py_DECREF(tmp);
            return NULL;
        }

        args_ptr[i] = ((PyJit_value *)tmp)->obj->raw();
        Py_DECREF(tmp);
    }

    try {
        retval = self->obj->insn_call_indirect(*(value->obj), signature->obj, args_ptr, num_args, flags);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_call_indirect_vtable(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    PyJit_type *signature;
    PyObject *py_args;
    int flags = 0;
    const char *keywords[] = {"value", "signature", "args", "flags", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O!O|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &signature, &py_args, &flags)) {
        return NULL;
    }

    if (jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect_vtable() argument 'signature' not a signature");
        return NULL;
    }

    if (PySequence_Check(py_args) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect_vtable() argument 'args' must be a sequence.");
        return NULL;
    }

    if (self->obj->raw() == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    int num_args = PySequence_Size(py_args);
    jit_value_t args_ptr[num_args];

    for (int i = 0; i < num_args; i++) {
        PyObject *tmp = PySequence_GetItem(py_args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_value_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Function.insn_call_indirect_vtable() argument 'args' must be a sequence of 'jit.Value'.");
            free(args_ptr);
            Py_DECREF(tmp);
            return NULL;
        }

        args_ptr[i] = ((PyJit_value *)tmp)->obj->raw();
        Py_DECREF(tmp);
    }

    try {
        retval = self->obj->insn_call_indirect_vtable(*(value->obj), signature->obj, args_ptr, num_args, flags);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_call_native(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    char const *name;
    PyObject *native_func;
    PyJit_type *signature;
    PyObject *py_args;
    int flags = 0;
    const char *keywords[] = {"name", "native_func", "signature", "args", "flags", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sOO!O|i", (char **) keywords, &name, &native_func, &PyJit_type_Type, &signature, &py_args, &flags)) {
        return NULL;
    }

    if (jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_native() argument 'signature' not a signature");
        return NULL;
    }

    if (PySequence_Check(py_args) == 0) {
        PyErr_SetString(PyExc_TypeError, "Function.insn_call_native() argument 'args' must be a sequence.");
        return NULL;
    }

    if (self->obj->raw() == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    int num_args = PySequence_Size(py_args);
    jit_value_t args_ptr[num_args];

    for (int i = 0; i < num_args; i++) {
        PyObject *tmp = PySequence_GetItem(py_args, i);

        if (PyObject_IsInstance(tmp, (PyObject *)&PyJit_value_Type) == 0) {
            PyErr_SetString(PyExc_TypeError, "Function.insn_call_native() argument 'args' must be a sequence of 'jit.Value'.");
            free(args_ptr);
            Py_DECREF(tmp);
            return NULL;
        }

        args_ptr[i] = ((PyJit_value *)tmp)->obj->raw();
        Py_DECREF(tmp);
    }

    try {
        retval = self->obj->insn_call_native(name, PyLong_AsVoidPtr(native_func), signature->obj, args_ptr, num_args, flags);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_import(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_import(*(value->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyObject *
PyJit_function_insn_return(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value = NULL;
    const char *keywords[] = {"value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "|O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }

    try {
        self->obj->insn_return((value ? (*(value->obj)) : NULL));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_return_ptr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *value;
    PyJit_type *type;
    const char *keywords[] = {"value", "type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &type)) {
        return NULL;
    }

    try {
        self->obj->insn_return_ptr(*(value->obj), type->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

PyObject *
PyJit_function_insn_default_return(PyJit_function *self)
{
    try {
        self->obj->insn_default_return();
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_memcpy(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *dest, *src, *size;
    const char *keywords[] = {"dest", "src", "size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &src, &PyJit_value_Type, &size)) {
        return NULL;
    }

    try {
        self->obj->insn_memcpy(*(dest->obj), *(src->obj), *(size->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_memmove(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *dest, *src, *size;
    const char *keywords[] = {"dest", "src", "size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &src, &PyJit_value_Type, &size)) {
        return NULL;
    }

    try {
        self->obj->insn_memmove(*((PyJit_value *) dest)->obj, *((PyJit_value *) src)->obj, *((PyJit_value *) size)->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_memset(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_value *dest, *value, *size;
    const char *keywords[] = {"dest", "value", "size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &value, &PyJit_value_Type, &size)) {
        return NULL;
    }

    try {
        self->obj->insn_memset(*((PyJit_value *) dest)->obj, *((PyJit_value *) value)->obj, *((PyJit_value *) size)->obj);
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
PyJit_function_insn_alloca(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    jit_value retval;
    PyJit_value *size;
    const char *keywords[] = {"size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &size)) {
        return NULL;
    }

    try {
        retval = self->obj->insn_alloca(*(size->obj));
    } catch (jit_build_exception &e) {
        PyErr_SetString(PyExc_RuntimeError, "Function object not initialized.");
        return NULL;
    }

    return convert_c2py__jit_value(&retval);
}

static PyMethodDef PyJit_function_methods[] = {
    {(char *) "build",                        (PyCFunction) PyJit_function__PythonHelper::py_build,                  METH_NOARGS,                NULL },
    {(char *) "create_signature",             (PyCFunction) PyJit_function__PythonHelper::py_create_signature,       METH_NOARGS,                NULL },
    {(char *) "fail",                         (PyCFunction) PyJit_function__PythonHelper::py_fail,                   METH_NOARGS,                NULL },
    {(char *) "out_of_memory",                (PyCFunction) PyJit_function__PythonHelper::py_out_of_memory,          METH_NOARGS,                NULL },
    {(char *) "build_start",                  (PyCFunction) PyJit_function_build_start,                              METH_NOARGS,                NULL },
    {(char *) "build_end",                    (PyCFunction) PyJit_function_build_end,                                METH_NOARGS,                NULL },
    {(char *) "compile",                      (PyCFunction) PyJit_function_compile,                                  METH_NOARGS,                NULL },
    {(char *) "create",                       (PyCFunction) PyJit_function_create,                                   METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "dump",                         (PyCFunction) PyJit_function_dump,                                     METH_NOARGS,                NULL },
    {(char *) "get_param",                    (PyCFunction) PyJit_function_get_param,                                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "get_struct_pointer",           (PyCFunction) PyJit_function_get_struct_pointer,                       METH_NOARGS,                NULL },
    {(char *) "new_constant",                 (PyCFunction) PyJit_function_new_constant,                             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "new_label",                    (PyCFunction) PyJit_function_new_label,                                METH_NOARGS,                NULL },
    {(char *) "new_value",                    (PyCFunction) PyJit_function_new_value,                                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_label",                   (PyCFunction) PyJit_function_insn_label,                               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_new_block",               (PyCFunction) PyJit_function_insn_new_block,                           METH_NOARGS,                NULL },
    {(char *) "insn_load",                    (PyCFunction) PyJit_function_insn_load,                                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_dup",                     (PyCFunction) PyJit_function_insn_dup,                                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_load_small",              (PyCFunction) PyJit_function_insn_load_small,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_store",                   (PyCFunction) PyJit_function_insn_store,                               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_load_relative",           (PyCFunction) PyJit_function_insn_load_relative,                       METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_store_relative",          (PyCFunction) PyJit_function_insn_store_relative,                      METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_add_relative",            (PyCFunction) PyJit_function_insn_add_relative,                        METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_load_elem",               (PyCFunction) PyJit_function_insn_load_elem,                           METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_load_elem_address",       (PyCFunction) PyJit_function_insn_load_elem_address,                   METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_store_elem",              (PyCFunction) PyJit_function_insn_store_elem,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_check_null",              (PyCFunction) PyJit_function_insn_check_null,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_add",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_add>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_add_ovf",                 (PyCFunction) PyJit_function_insn_binop<jit_insn_add_ovf>,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sub",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_sub>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sub_ovf",                 (PyCFunction) PyJit_function_insn_binop<jit_insn_sub_ovf>,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_mul",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_mul>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_mul_ovf",                 (PyCFunction) PyJit_function_insn_binop<jit_insn_mul_ovf>,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_div",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_div>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_rem",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_rem>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_rem_ieee",                (PyCFunction) PyJit_function_insn_binop<jit_insn_rem_ieee>,            METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_neg",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_neg>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_and",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_and>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_or",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_or>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_xor",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_xor>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_not",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_not>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_shl",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_shl>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_shr",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_shr>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_ushr",                    (PyCFunction) PyJit_function_insn_binop<jit_insn_ushr>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sshr",                    (PyCFunction) PyJit_function_insn_binop<jit_insn_sshr>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_eq",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_eq>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_ne",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_ne>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_lt",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_lt>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_le",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_le>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_gt",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_gt>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_ge",                      (PyCFunction) PyJit_function_insn_binop<jit_insn_ge>,                  METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_cmpl",                    (PyCFunction) PyJit_function_insn_binop<jit_insn_cmpl>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_cmpg",                    (PyCFunction) PyJit_function_insn_binop<jit_insn_cmpg>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_to_bool",                 (PyCFunction) PyJit_function_insn_uniop<jit_insn_to_bool>,             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_to_not_bool",             (PyCFunction) PyJit_function_insn_uniop<jit_insn_to_not_bool>,         METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_acos",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_acos>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_asin",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_asin>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_atan",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_atan>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_atan2",                   (PyCFunction) PyJit_function_insn_binop<jit_insn_atan2>,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_ceil",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_ceil>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_cos",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_cos>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_cosh",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_cosh>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_exp",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_exp>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_floor",                   (PyCFunction) PyJit_function_insn_uniop<jit_insn_floor>,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_log",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_log>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_log10",                   (PyCFunction) PyJit_function_insn_uniop<jit_insn_log10>,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_pow",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_pow>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_rint",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_rint>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_round",                   (PyCFunction) PyJit_function_insn_uniop<jit_insn_round>,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sin",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_sin>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sinh",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_sinh>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sqrt",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_sqrt>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_tan",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_tan>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_tanh",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_tanh>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_trunc",                   (PyCFunction) PyJit_function_insn_uniop<jit_insn_trunc>,               METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_is_nan",                  (PyCFunction) PyJit_function_insn_uniop<jit_insn_is_nan>,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_is_finite",               (PyCFunction) PyJit_function_insn_uniop<jit_insn_is_finite>,           METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_is_inf",                  (PyCFunction) PyJit_function_insn_uniop<jit_insn_is_inf>,              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_abs",                     (PyCFunction) PyJit_function_insn_uniop<jit_insn_abs>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_min",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_min>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_max",                     (PyCFunction) PyJit_function_insn_binop<jit_insn_max>,                 METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_sign",                    (PyCFunction) PyJit_function_insn_uniop<jit_insn_sign>,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_branch",                  (PyCFunction) PyJit_function_insn_branch,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_branch_if",               (PyCFunction) PyJit_function_insn_branch_if,                           METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_branch_if_not",           (PyCFunction) PyJit_function_insn_branch_if_not,                       METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_jump_table",              (PyCFunction) PyJit_function_insn_jump_table,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_address_of",              (PyCFunction) PyJit_function_insn_address_of,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_address_of_label",        (PyCFunction) PyJit_function_insn_address_of_label,                    METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_convert",                 (PyCFunction) PyJit_function_insn_convert,                             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_call",                    (PyCFunction) PyJit_function_insn_call,                                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_call_indirect",           (PyCFunction) PyJit_function_insn_call_indirect,                       METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_call_indirect_vtable",    (PyCFunction) PyJit_function_insn_call_indirect_vtable,                METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_call_native",             (PyCFunction) PyJit_function_insn_call_native,                         METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_import",                  (PyCFunction) PyJit_function_insn_import,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_return",                  (PyCFunction) PyJit_function_insn_return,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_return_ptr",              (PyCFunction) PyJit_function_insn_return_ptr,                          METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_default_return",          (PyCFunction) PyJit_function_insn_default_return,                      METH_NOARGS,                NULL },
    {(char *) "insn_memcpy",                  (PyCFunction) PyJit_function_insn_memcpy,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_memmove",                 (PyCFunction) PyJit_function_insn_memmove,                             METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_memset",                  (PyCFunction) PyJit_function_insn_memset,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {(char *) "insn_alloca",                  (PyCFunction) PyJit_function_insn_alloca,                              METH_KEYWORDS|METH_VARARGS, NULL },
    {NULL, NULL, 0, NULL}
};

static PyObject *
PyJit_function__tp_call(PyJit_function *self, PyObject *args, PyObject *UNUSED(kwargs))
{
    void *result;
    PyObject *py_return;
    jit_type_t signature = self->obj->signature();
    int num_args = jit_type_num_params(signature);
    void *args_ptr[num_args];

    if (PySequence_Size(args) != num_args) {
        PyErr_Format(PyExc_ValueError, "function requires %i arguments", num_args);
        return NULL;
    }

    for (int i = 0; i < num_args; i++) {
        PyObject *tmp = PySequence_GetItem(args, i);
        jit_type_t arg = jit_type_get_param(signature, i);

        switch (jit_type_get_kind(jit_type_normalize(arg))) {
            case JIT_TYPE_SBYTE:
            case JIT_TYPE_UBYTE:
            case JIT_TYPE_SHORT:
            case JIT_TYPE_USHORT:
            case JIT_TYPE_INT:
            case JIT_TYPE_UINT:
#if PY_VERSION_HEX < 0x03000000
                args_ptr[i] = malloc(sizeof(jit_int));
                *((jit_int *)args_ptr[i]) = PyInt_AsLong(tmp);
                break;
#endif
            case JIT_TYPE_NINT:
            case JIT_TYPE_NUINT:
            case JIT_TYPE_LONG:
            case JIT_TYPE_ULONG:
                args_ptr[i] = malloc(sizeof(jit_nint));
                *((jit_nint *)args_ptr[i]) = PyLong_AsLong(tmp);
                break;
            case JIT_TYPE_FLOAT32:
                args_ptr[i] = malloc(sizeof(jit_float32));
                *((jit_float32 *)args_ptr[i]) = PyFloat_AsDouble(tmp);
                break;
            case JIT_TYPE_NFLOAT:
            case JIT_TYPE_FLOAT64:
                args_ptr[i] = malloc(sizeof(jit_float64));
                *((jit_float64 *)args_ptr[i]) = PyFloat_AsDouble(tmp);
                break;
            /* probably not the correct way to pass structs and unions */
            case JIT_TYPE_STRUCT:
            case JIT_TYPE_UNION:
            case JIT_TYPE_PTR:
                args_ptr[i] = malloc(sizeof(void *));
                args_ptr[i] = PyLong_AsVoidPtr(tmp);
                break;
            default:
            {
                for (int j = 0; j < i; j++) {
                    free(args_ptr[j]);
                }
                PyErr_Format(PyExc_TypeError, "argument %i type not supported", i);
                return NULL;
            }
        }
    }

	result = malloc(sizeof(jit_nfloat)); // easier way than use malloc?

    if (self->obj->apply(args_ptr, result)) {
        jit_type_t kind = jit_type_normalize(jit_type_get_return(signature));
        switch (jit_type_get_kind(kind)) {
            case JIT_TYPE_SBYTE:
                py_return = PyInt_FromLong(*((jit_sbyte *)result));
                break;
            case JIT_TYPE_UBYTE:
                py_return = PyInt_FromLong(*((jit_ubyte *)result));
                break;
            case JIT_TYPE_SHORT:
                py_return = PyInt_FromLong(*((jit_short *)result));
                break;
            case JIT_TYPE_USHORT:
                py_return = PyInt_FromLong(*((jit_ushort *)result));
                break;
            case JIT_TYPE_INT:
                py_return = PyInt_FromLong(*((jit_int *)result));
                break;
            case JIT_TYPE_UINT:
                py_return = PyInt_FromLong(*((jit_uint *)result));
                break;
            case JIT_TYPE_LONG:
            case JIT_TYPE_NINT:
                py_return = PyLong_FromLong(*((jit_nint *)result));
                break;
            case JIT_TYPE_ULONG:
            case JIT_TYPE_NUINT:
                py_return = PyLong_FromLong(*((jit_nuint *)result));
                break;
            case JIT_TYPE_FLOAT32:
                py_return = PyFloat_FromDouble(*((jit_float32 *)result));
                break;
            case JIT_TYPE_FLOAT64:
                py_return = PyFloat_FromDouble(*((jit_float64 *)result));
                break;
            case JIT_TYPE_NFLOAT:
                py_return = PyFloat_FromDouble(*((jit_nfloat *)result));
                break;
            case JIT_TYPE_STRUCT:
            case JIT_TYPE_UNION:
            case JIT_TYPE_PTR:
                py_return = PyLong_FromVoidPtr((void *)(*((long *)result)));
                break;
            default:
                Py_INCREF(Py_None);
                py_return = Py_None;
                break;
        }
    } else {
        PyErr_SetString(PyExc_RuntimeError, "unable to call function");
        py_return = NULL;
    }
    /* cleanup */
    for (int i = 0; i < num_args; i++) {
        free(args_ptr[i]);
    }
    free(result);

    return py_return;
}

static int
PyJit_function__tp_init(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyJit_context *context;
    PyJit_type *signature = NULL;
    const char *keywords[] = {"context", "signature", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|O!", (char **) keywords, &PyJit_context_Type, &context, &PyJit_type_Type, &signature)) {
        return -1;
    }

    if (signature && jit_type_is_signature(signature->obj) == 0) {
        PyErr_SetString(PyExc_TypeError, "'jit.Type' not a signature");
        return -1;
    }

    if (Py_TYPE(self) != &PyJit_function_Type) {
        if (signature) {
            self->obj = new PyJit_function__PythonHelper(*(context->obj), signature->obj);
        } else {
            self->obj = new PyJit_function__PythonHelper(*(context->obj));
        }
        ((PyJit_function__PythonHelper*) self->obj)->set_pyobj((PyObject *)self);
    } else {
        if (signature) {
            self->obj = new jit_function(*(context->obj), signature->obj);
        } else {
            self->obj = new jit_function(*(context->obj));
        }
    }

    self->flags = JIT_FLAG_NONE;

    return 0;
}

static void
PyJit_function__tp_clear(PyJit_function *self)
{
    Py_CLEAR(self->inst_dict);

    jit_function *tmp = self->obj;

    self->obj = NULL;

    if (!(self->flags & JIT_FLAG_OBJECT_NOT_OWNED)) {
        delete tmp;
    }
}

static int
PyJit_function__tp_traverse(PyJit_function *self, visitproc visit, void *arg)
{
    Py_VISIT(self->inst_dict);

    if (self->obj && typeid(*self->obj).name() == typeid(PyJit_function__PythonHelper).name() )
        Py_VISIT((PyObject *) self);

    return 0;
}

static void
PyJit_function__tp_dealloc(PyJit_function *self)
{
    PyJit_function__tp_clear(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_function_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Function",                                      /* tp_name */
    sizeof(PyJit_function),                                       /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_function__tp_dealloc,                       /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)NULL,                                     /* tp_as_sequence */
    (PyMappingMethods*)NULL,                                      /* tp_as_mapping */
    (hashfunc)NULL,                                               /* tp_hash */
    (ternaryfunc)PyJit_function__tp_call,                         /* tp_call */
    (reprfunc)NULL,                                               /* tp_str */
    (getattrofunc)NULL,                                           /* tp_getattro */
    (setattrofunc)NULL,                                           /* tp_setattro */
    (PyBufferProcs*)NULL,                                         /* tp_as_buffer */
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_GC,    /* tp_flags */
    NULL,                                                         /* Documentation string */
    (traverseproc)PyJit_function__tp_traverse,                    /* tp_traverse */
    (inquiry)PyJit_function__tp_clear,                            /* tp_clear */
    (richcmpfunc)NULL,                                            /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)NULL,                                            /* tp_iter */
    (iternextfunc)NULL,                                           /* tp_iternext */
    (struct PyMethodDef*)PyJit_function_methods,                  /* tp_methods */
    (struct PyMemberDef*)0,                                       /* tp_members */
    PyJit_function__getsets,                                      /* tp_getset */
    NULL,                                                         /* tp_base */
    NULL,                                                         /* tp_dict */
    (descrgetfunc)NULL,                                           /* tp_descr_get */
    (descrsetfunc)NULL,                                           /* tp_descr_set */
    offsetof(PyJit_function, inst_dict),                          /* tp_dictoffset */
    (initproc)PyJit_function__tp_init,                            /* tp_init */
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
PyJit_function_list__tp_iter(PyJit_function_list *self)
{
    Py_INCREF(self);
    return (PyObject *) self;
}

static PyObject *
PyJit_function_list__tp_iternext(PyJit_function_list *self)
{
    jit_function_t next = jit_function_next(self->context->obj->raw(), (self->prev ? self->prev->obj->raw() : NULL));

    if (next == NULL) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    jit_function *func = jit_function::from_raw(next);

    if (func == NULL) {
        func = new jit_function(next);
    }

    PyObject *retval = convert_c2py__jit_function(func, 1);

    Py_XDECREF(self->prev);

    self->prev = (PyJit_function *)retval;

    Py_XINCREF(self->prev);

    return retval;
}

static PyObject *
PyJit_function_list_mp_subscript(PyObject *self, PyObject *key)
{
    jit_context_t context = ((PyJit_function_list *)self)->context->obj->raw();

    if (!PyInt_Check(key)) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    int index = PyInt_AsLong(key);

    jit_function_t func = (index >= 0 ? jit_function_next(context, NULL) : jit_function_previous(context, NULL));

    if (index > 0) {
        for (int i = index; i > 0 && func != NULL; i--) {
            func = jit_function_next(context, func);
        }
    } else if (index < 0) {
        for (int i = index; i < -1 && func != NULL; i++) {
            func = jit_function_previous(context, func);
        }
    }

    if (func == NULL) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    jit_function *retval = jit_function::from_raw(func);

    if (retval == NULL) {
        retval = new jit_function(func);
    }

    return convert_c2py__jit_function(retval, 1);
}

static PyMappingMethods PyJit_function_list_as_mapping = {
    NULL,                                                         /* mp_length */
    PyJit_function_list_mp_subscript,                             /* mp_subscript */
    NULL,                                                         /* mp_ass_subscript */
};

static void
PyJit_function_list__tp_clear(PyJit_function_list *self)
{
    Py_CLEAR(self->context);
    Py_CLEAR(self->prev);
}

static void
PyJit_function_list__tp_dealloc(PyJit_function_list *self)
{
    PyJit_function_list__tp_clear(self);

    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_function_list_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.FunctionIter",                                  /* tp_name */
    sizeof(PyJit_function_list),                                  /* tp_basicsize */
    0,                                                            /* tp_itemsize */
    /* methods */
    (destructor)PyJit_function_list__tp_dealloc,                  /* tp_dealloc */
    (printfunc)0,                                                 /* tp_print */
    (getattrfunc)NULL,                                            /* tp_getattr */
    (setattrfunc)NULL,                                            /* tp_setattr */
    (cmpfunc)NULL,                                                /* tp_compare */
    (reprfunc)NULL,                                               /* tp_repr */
    (PyNumberMethods*)NULL,                                       /* tp_as_number */
    (PySequenceMethods*)NULL,                                     /* tp_as_sequence */
    (PyMappingMethods*)&PyJit_function_list_as_mapping,           /* tp_as_mapping */
    (hashfunc)NULL,                                               /* tp_hash */
    (ternaryfunc)NULL,                                            /* tp_call */
    (reprfunc)NULL,                                               /* tp_str */
    (getattrofunc)NULL,                                           /* tp_getattro */
    (setattrofunc)NULL,                                           /* tp_setattro */
    (PyBufferProcs*)NULL,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_GC,                        /* tp_flags */
    NULL,                                                         /* Documentation string */
    (traverseproc)NULL,                                           /* tp_traverse */
    (inquiry)PyJit_function_list__tp_clear,                       /* tp_clear */
    (richcmpfunc)NULL,                                            /* tp_richcompare */
    0,                                                            /* tp_weaklistoffset */
    (getiterfunc)PyJit_function_list__tp_iter,                    /* tp_iter */
    (iternextfunc)PyJit_function_list__tp_iternext,               /* tp_iternext */
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

