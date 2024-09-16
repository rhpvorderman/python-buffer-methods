/*
Copyright (C) 2024 Leiden University Medical Center
This file is part of python-buffer-methods

python-buffer-methods is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

python-buffer-methods is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with python-buffer-methods.  If not, see <https://www.gnu.org/licenses/
*/

#define PY_SSIZE_T_CLEAN
#include "Python.h"

static PyMethodDef _buffer_methods[] = {
    {NULL},
};

static struct PyModuleDef buffer_methods_module = {
    PyModuleDef_HEAD_INIT,
    "_buffer_methods", /* name of module */
    NULL,              /* module documentation, may be NULL */
    -1,                /* size of per-interpreter state of the module,
                          or -1 if the module keeps state in global variables. */
    _buffer_methods,
};

PyMODINIT_FUNC
PyInit__buffer_methods(void)
{
    PyObject *m;

    m = PyModule_Create(&buffer_methods_module);
    if (m == NULL) {
        return NULL;
    }
    return m;
}