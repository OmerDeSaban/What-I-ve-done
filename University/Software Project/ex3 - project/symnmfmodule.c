#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"


extern int N;  /* The number of given points */
extern int dim;    /* The shared dimension of all points */
extern int K;  /* The desired number of symnmf clusters (or centroids) */
extern double **points_array;  /* The given points */
extern double **A; /* The similarity matrix */
extern double **D; /* The diagonal degree matrix */
extern double **W; /* The normalized similarity matrix */
extern double **H; /* The decomposition matrix */


/** Parses a Python matrix, represented by a Python list of lists of floats, into a C array of arrays of
 *  doubles.
 *  @param c_mat A pointer to the C array we wish to initialize.
 *  @param py_mat A pointer to the Python matrix.
 *  @param columns The number of columns in the given Python matrix.
 *  @return An integer denoting if the parsing was successful, where:
 *    1 means that the parsing failed ;
 *    0 means that it was successful. */
int parseMat(double ***c_mat, PyObject *py_mat, int columns) {
  int i, j;
  PyObject *py_row, *py_item;
  if (allocateMatDouble(c_mat, N) == 1) { return 1; }  /* Memory allocation error */
  for (i = 0; i < N; i++) {
    if (allocateRowDouble(&((*c_mat)[i]), columns, 1) == 1) {  /* Memory allocation error */
      freeArrayDouble(*c_mat, i);
      return 1;
    }
    py_row = PyList_GetItem(py_mat, i);
    if (!py_row) {  /* General error */
      freeArrayDouble(*c_mat, (i + 1));
      return 1;
    }
    for (j = 0; j < columns; j++) {
      py_item = PyList_GetItem(py_row, j);
      if (!py_item) { /* General error */
        freeArrayDouble(*c_mat, (i + 1));
        return 1;
      }
      (*c_mat)[i][j] = PyFloat_AsDouble(py_item);
      if (PyErr_Occurred()) { /* General error */
        freeArrayDouble(*c_mat, (i + 1));
        return 1;
      }
    }
  }
  return 0;
}

/** Parse the given points from a Python list to a C array of arrays of doubles.
 *  Updates the global points_array variable to be the parsed points.
 *  @param self The module object.
 *  @param args The given Python arguments. Should only contain a Python list of lists of floats.
 *  @return An integer denoting if the parsing was successful, where:
 *    1 means that the parsing failed ;
 *    0 means that it was successful. */
int PyToC_Points(PyObject *self, PyObject *args) {
  PyObject *py_points, *py_point;
  /* This parses the Python arguments into a PyObject (O) variable named py_points : */
  if (!PyArg_ParseTuple(args, "O", &py_points)) { return 1; } /* Argument parsing error */
  N = (int)PyList_Size(py_points);
  if ((N <= 0) || (PyErr_Occurred())) { return 1; } /* Invalid N or general error */
  py_point = PyList_GetItem(py_points, 0);
  if (!py_point) { return 1; }  /* General error */
  dim = (int)PyList_Size(py_point);
  if ((dim <= 0) || (PyErr_Occurred())) { return 1; } /* Invalid dim or general error */
  points_array = NULL;
  if (parseMat(&points_array, py_points, dim) == 1) { return 1; } /* Parsing error */
  return 0;
}

/** Parse each of the given H and W matrices from a Python list into C array of arrays of doubles.
 *  Updates the global H and W variables to be the parsed Python H and W matrices, respectively.
 *  @param self The module object.
 *  @param args The given Python arguments. Should only contain a Python list of lists of floats, that
 *    should represent the initialized decomposition matrix, and another Python list of lists of floats,
 *    that should represent the normalized similarity matrix.
 *  @return An integer denoting if the parsing was successful, where:
 *    1 means that the parsing failed ;
 *    0 means that it was successful. */
int PyToC_symNMF(PyObject *self, PyObject *args) {
  PyObject *py_H, *py_W, *py_row;
  /* This parses the Python arguments into 2 PyObject (O) variables named py_H and py_W : */
  if (!PyArg_ParseTuple(args, "OO", &py_H, &py_W)) { return 1; } /* Parsing error */
  if ((!py_H) || (!py_W)) { return 1; } /* General error */
  N = (int)PyList_Size(py_H);
  if ((N <= 0) || (PyErr_Occurred())) { return 1; } /* Invalid N or general error */
  py_row = PyList_GetItem(py_H, 0);
  if (!py_row) { return 1; }  /* General error */
  K = (int)PyList_Size(py_row);
  if ((K <= 0) || (K >= N) || (PyErr_Occurred())) { return 1; } /* Invalid K or general error */
  H = NULL;
  if (parseMat(&H, py_H, K) == 1) { return 1; } /* Parsing error */
  W = NULL;
  if (parseMat(&W, py_W, N) == 1) { /* Parsing error */
    freeAll();
    return 1;
  }
  return 0;
}

/** Parse the result matrix from a C object into a Python list.
 *  @param result The result matrix, before rounding its elements.
 *  @param columns The number of columns in the result matrix.
 *  @return The result matrix as a Python list, or NULL if an error occurred. */
PyObject *CToPy_Result(double **Result, int columns) {
  int i, j;
  PyObject *py_res, *py_row, *py_float;
  py_res = PyList_New(N);
  if ((!py_res) || (!Result)) { /* Memory allocation, or algorithm error */
    freeAll();
    return NULL;
  }
  for (i = 0; i < N; i++) { 
    py_row = PyList_New(columns);
    if ((!py_row) || (!Result[i])) {  /* Memory allocation error */
      Py_DECREF(py_res);
      freeAll();
      return NULL;
    }
    for (j = 0; j < columns; j++) {
      py_float = PyFloat_FromDouble(Result[i][j]);
      if (!py_float) {  /* General error */
        Py_DECREF(py_row);
        Py_DECREF(py_res);
        freeAll();
        return NULL;
      }
      PyList_SET_ITEM(py_row, j, py_float);
    }
    PyList_SET_ITEM(py_res, i, py_row);
  }
  freeAll(); /* free all allocated memory */
  return py_res;
}

/** Python's sym() function.
 *  @param self The module object.
 *  @param args The given Python arguments.
 *  @return The result similarity matrix as a Python list, after rounding all its elements. */
PyObject *symPy(PyObject *self, PyObject *args) {
  if (PyToC_Points(self, args) == 1) { return NULL; } /* Parsing error */
  if (sym() == 1) { return NULL; }  /* Algorithm error */
  return CToPy_Result(A, N);
}

/** Python's ddg() function.
 *  @param self The module object.
 *  @param args The given Python arguments.
 *  @return The result diagonal degree matrix as a Python list, after rounding all its elements.  */
PyObject *ddgPy(PyObject *self, PyObject *args) {
  if (PyToC_Points(self, args) == 1) { return NULL; } /* Parsing error */
  if (ddg() == 1) { return NULL; }  /* Algorithm error */
  return CToPy_Result(D, N);
}

/** Python's norm() function.
 *  @param self The module object.
 *  @param args The given Python arguments.
 *  @return The result normalized similarity matrix as a Python list, after rounding all its elements.  */
PyObject *normPy(PyObject *self, PyObject *args) {
  if (PyToC_Points(self, args) == 1) { return NULL; } /* Parsing error */
  if (norm() == 1) { return NULL; } /* Algorithm error */
  return CToPy_Result(W, N);
}

/** Python's symnmf() function.
 *  @param self The module object.
 *  @param args The given Python arguments.
 *  @return The result decomposition matrix as a Python list, after rounding all its elements.  */
PyObject *symnmfPy(PyObject *self, PyObject *args) {
  if (PyToC_symNMF(self, args) == 1) { return NULL; } /* Parsing error */
  if (symnmf() == 1) { return NULL; } /* Algorithm error */
  return CToPy_Result(H, K);
}

static PyMethodDef symnmfMethods[] = {
    {"sym",
      (PyCFunction) symPy,
      METH_VARARGS,
      PyDoc_STR("Calculates and returns the similarity matrix. Does not round its elements.\nArgs:\n    points_list (List[list[float]]): All the initial points.\nReturns:\n    List[List[str]]: The calculated similarity matrix.")},
    {"ddg",
      (PyCFunction) ddgPy,
      METH_VARARGS,
      PyDoc_STR("Calculates and returns the diagonal degree matrix. Does not round its elements.\nArgs:\n    points_list (List[list[float]]): All the initial points.\nReturns:\n    List[List[str]]: The calculated diagonal degree matrix.")},
    {"norm",
      (PyCFunction) normPy,
      METH_VARARGS,
      PyDoc_STR("Calculates and returns the normalized similarity matrix. Does not round its elements.\nArgs:\n    points_list (List[list[float]]): All the initial points.\nReturns:\n    List[List[str]]: The calculated normalized similarity matrix.")},
    {"symnmf",
      (PyCFunction) symnmfPy,
      METH_VARARGS,
      PyDoc_STR("Calculates and returns the decomposition matrix. Does not round its elements.\nArgs:\n    H (List[list[float]]): The initialized decomposition matrix.\n    W (List[List[float]]): The normalized similarity matrix.\nReturns:\n    List[List[str]]: The calculated decomposition matrix.")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmod",
    NULL,
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_symnmfmod(void)
{
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) { return NULL; }
    return m;
}
