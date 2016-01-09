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

void
py_jit_meta_free_func(void *data)
{
    Py_XDECREF((PyObject *)data);
}

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit",
    NULL,
    -1,
    NULL,
};
#endif


#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_abi_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.abi",
    NULL,
    -1,
    NULL,
};
#endif

static PyObject *
initjit_abi(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_abi_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.abi", NULL, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "cdecl",    jit_abi_cdecl);
    PyModule_AddIntConstant(m, (char *) "vararg",   jit_abi_vararg);
    PyModule_AddIntConstant(m, (char *) "stdcall",  jit_abi_stdcall);
    PyModule_AddIntConstant(m, (char *) "fastcall", jit_abi_fastcall);
    return m;
}

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_call_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.call",
    NULL,
    -1,
    NULL,
};
#endif

static PyObject *
initjit_call(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_call_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.call", NULL, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "nothrow",    JIT_CALL_NOTHROW);
    PyModule_AddIntConstant(m, (char *) "noreturn",   JIT_CALL_NORETURN);
    PyModule_AddIntConstant(m, (char *) "tail",       JIT_CALL_TAIL);
    return m;
}

#if PY_VERSION_HEX >= 0x03000000
    #define MOD_ERROR NULL
    #define MOD_INIT(name) PyObject* PyInit_##name(void)
    #define MOD_RETURN(val) val
#else
    #define MOD_ERROR
    #define MOD_INIT(name) void init##name(void)
    #define MOD_RETURN(val)
#endif
#if defined(__cplusplus)
extern "C"
#endif
#if defined(__GNUC__) && __GNUC__ >= 4
__attribute__ ((visibility("default")))
#endif
MOD_INIT(jit)
{
    PyObject *m;
    PyObject *submodule;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_moduledef);
    #else
    m = Py_InitModule3((char *) "jit", NULL, NULL);
    #endif
    if (m == NULL) {
        return MOD_ERROR;
    }

    submodule = initjit_abi();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "abi", submodule);

    submodule = initjit_call();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "call", submodule);

    submodule = initjit_type();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "type", submodule);

    submodule = initjit_dynamic();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "dynamic", submodule);

    /* Register the 'jit_block_t' class */
    if (PyType_Ready(&PyJit_block_Type)) {
        return MOD_ERROR;
    }
    if (PyType_Ready(&PyJit_block_list_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Block", (PyObject *) &PyJit_block_Type);

    /* Register the 'jit_context' class */
    if (PyType_Ready(&PyJit_context_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Context", (PyObject *) &PyJit_context_Type);

    /* Register the 'jit_function' class */
    if (PyType_Ready(&PyJit_function_Type)) {
        return MOD_ERROR;
    }
    if (PyType_Ready(&PyJit_function_list_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Function", (PyObject *) &PyJit_function_Type);

    /* Register the 'jit_jump_table' class */
    if (PyType_Ready(&PyJit_jump_table_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "JumpTable", (PyObject *) &PyJit_jump_table_Type);

    /* Register the 'jit_label' class */
    if (PyType_Ready(&PyJit_label_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Label", (PyObject *) &PyJit_label_Type);

    /* Register the 'jit_value' class */
    if (PyType_Ready(&PyJit_value_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Value", (PyObject *) &PyJit_value_Type);

    return MOD_RETURN(m);
}
