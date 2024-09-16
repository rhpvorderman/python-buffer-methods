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

#include "c_methods.h"

static inline Py_ssize_t
buffer_length(Py_buffer buffer)
{
    Py_ssize_t itemsize = buffer.itemsize;
    Py_ssize_t buffer_length = buffer.len;
    Py_ssize_t buffer_mod = 0;
    Py_ssize_t size;
    switch (itemsize) {
        case 8:
            size = buffer_length / 8;
            buffer_mod = buffer_length % 8;
            break;
        case 4:
            size = buffer_length / 4;
            buffer_mod = buffer_length % 4;
            break;
        case 2:
            size = buffer_length / 2;
            buffer_mod = buffer_length % 2;
            break;
        case 1:
            size = buffer_length;
            break;
        default:
            PyErr_Format(PyExc_BufferError,
                         "Ivalid buffer: Unknown itemsize %d", itemsize);
            return -1;
    }
    if (buffer_mod) {
        PyErr_Format(PyExc_BufferError,
                     "Invalid buffer: Size %zd not divisible by itemsize %zd",
                     size, itemsize);
        return -1;
    }
    return size;
}

static PyObject *
sum(PyObject *module, PyObject *args, PyObject *kwargs)
{
    PyObject *array_obj = NULL;
    Py_ssize_t start = 0;
    static char *args_format = "O|n:buffer_methods.sum";
    static char *keywords[] = {"", "start", NULL};
    if (PyArg_ParseTupleAndKeywords(args, kwargs, args_format, keywords,
                                    &array_obj, &start) < 0) {
        return NULL;
    }

    Py_buffer buf;
    PyObject *answer = NULL;
    if (PyObject_GetBuffer(array_obj, &buf,
                           PyBUF_ND | PyBUF_FORMAT | PyBUF_READ) == -1) {
        return NULL;
    }
    Py_ssize_t offset = buf.itemsize * start;
    if (offset > buf.len) {
        offset = buf.len;
    }
    void *numbers = (uint8_t *)buf.buf + offset;

    Py_ssize_t size = buffer_length(buf);
    if (size == -1) {
        PyBuffer_Release(&buf);
        return NULL;
    }
    if (start > size) {
        size = 0;
    }
    else {
        size -= start;
    }

    char format;
    if (buf.format == NULL) {
        format = 'B';
    }
    else {
        format = buf.format[0];
    }

    switch (format) {
        case 'B':
            answer = PyLong_FromUnsignedLongLong(_uint8_t_sum(numbers, size));
            break;
        case 'H':
            answer = PyLong_FromUnsignedLongLong(_uint16_t_sum(numbers, size));
            break;
        case 'I':
        case 'L':
            answer = PyLong_FromUnsignedLongLong(_uint32_t_sum(numbers, size));
            break;
        case 'Q':
            answer = PyLong_FromUnsignedLongLong(_uint64_t_sum(numbers, size));
            break;

        case 'b':
            answer = PyLong_FromLongLong(_int8_t_sum(numbers, size));
            break;
        case 'h':
            answer = PyLong_FromLongLong(_int16_t_sum(numbers, size));
            break;
        case 'i':
        case 'l':
            answer = PyLong_FromLongLong(_int32_t_sum(numbers, size));
            break;
        case 'q':
            answer = PyLong_FromLongLong(_int64_t_sum(numbers, size));
            break;

        case 'f':
            answer = PyFloat_FromDouble(_float_sum(numbers, size));
            break;
        case 'd':
            answer = PyFloat_FromDouble(_double_sum(numbers, size));
            break;

        default:
            PyErr_Format(PyExc_ValueError, "Unsupported buffer format: '%c'",
                         format);
            PyBuffer_Release(&buf);
            return NULL;
    }
    PyBuffer_Release(&buf);
    return answer;
}

static PyMethodDef _buffer_methods[] = {
    {"sum", (PyCFunction)sum, METH_VARARGS | METH_KEYWORDS, NULL},
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