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

#ifndef PYJIT_MODULE_H
#define PYJIT_MODULE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>


#if PY_VERSION_HEX >= 0x03000000
typedef void* cmpfunc;
//typedef void* ssizessizeargfunc;
//typedef void* ssizessizeobjargproc;
#define PyInt_Check(x) PyLong_Check(x)
#define PyInt_AsLong(x) PyLong_AsLong(x)
#define PyInt_FromLong(x) PyLong_FromLong(x)
#define PyString_Check(x) PyBytes_Check(x)
#define PyString_FromStringAndSize(x, y) PyUnicode_FromStringAndSize(x, y)
#define Py_TPFLAGS_CHECKTYPES 0 /* this flag doesn't exist in python 3 */
#endif

#if     __GNUC__ > 2
# define UNUSED(param) param __attribute__((__unused__))
#elif     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
# define UNUSED(param) __attribute__((__unused__)) param
#else
# define UNUSED(param) param
#endif  /* !__GNUC__ */

#ifndef _WrapperFlags_defined_
#define _WrapperFlags_defined_
typedef enum _PyJit_Flags {
   JIT_FLAG_NONE = 0,
   JIT_FLAG_OBJECT_NOT_OWNED = (1<<0),
} PyJit_Flags;
#endif

#include "jit/jit-plus.h"

typedef struct {
    PyObject_HEAD
    jit_block_t obj;
} PyJit_block;

typedef struct {
    PyObject_HEAD
    jit_context *obj;
} PyJit_context;

typedef struct {
    PyObject_HEAD
    jit_function *obj;
    PyObject *inst_dict;
    PyJit_Flags flags:8;
} PyJit_function;

typedef struct {
    PyObject_HEAD
    jit_jump_table *obj;
} PyJit_jump_table;

typedef struct {
    PyObject_HEAD
    jit_label *obj;
} PyJit_label;

typedef struct {
    PyObject_HEAD
    jit_type_t obj;
} PyJit_type;

typedef struct {
    PyObject_HEAD
    jit_value *obj;
} PyJit_value;

typedef struct {
    PyObject_HEAD
    PyJit_context *context;
    PyJit_function *prev;
} PyJit_function_list;

typedef struct {
    PyObject_HEAD
    PyJit_function *func;
    PyJit_block *prev;
} PyJit_block_list;

extern PyTypeObject PyJit_block_Type;
extern PyTypeObject PyJit_block_list_Type;
extern PyTypeObject PyJit_context_Type;
extern PyTypeObject PyJit_function_Type;
extern PyTypeObject PyJit_function_list_Type;
extern PyTypeObject PyJit_jump_table_Type;
extern PyTypeObject PyJit_label_Type;
extern PyTypeObject PyJit_type_Type;
extern PyTypeObject PyJit_value_Type;


PyObject *initjit_type(void);
PyObject *initjit_dynamic(void);

void py_jit_meta_free_func(void *data);

PyObject *convert_c2py__jit_type_t(jit_type_t cvalue, int copy);

PyObject *convert_c2py__jit_value(jit_value *cvalue);
int convert_py2c__jit_value(PyObject *value, jit_value *address);

PyObject *convert_c2py__jit_function(jit_function *cvalue, int copy);

class PyJit_function__PythonHelper : public jit_function
{
    public:
        PyObject *m_pyself;

        PyJit_function__PythonHelper(jit_context &context) : jit_function(context), m_pyself(NULL) {}

        PyJit_function__PythonHelper(jit_context &context, jit_type_t signature) : jit_function(context, signature), m_pyself(NULL) {}


        void set_pyobj(PyObject *pyobj)
        {
            Py_XDECREF(m_pyself);
            Py_INCREF(pyobj);
            m_pyself = pyobj;
        }

        virtual ~PyJit_function__PythonHelper() { Py_CLEAR(m_pyself); }

        static PyObject * py_create_signature(PyJit_function *self);

        static PyObject * py_build(PyJit_function *self);

        static PyObject * py_fail(PyJit_function *self);
        inline void fail__parent_caller() { jit_function::fail(); }

        static PyObject * py_out_of_memory(PyJit_function *self);
        inline void out_of_memory__parent_caller() { jit_function::out_of_memory(); }

        virtual void build();

        virtual jit_type_t create_signature();
};

#endif /* PYJIT_MODULE_H */
