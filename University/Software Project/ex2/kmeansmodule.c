#define PY_SSIZE_T_CLEAN
#include <Python.h>
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** A struct that holds the value of the current coordinate and pointer to the next one. */
struct coordinate {
    double value;
    struct coordinate* next;
};

/** A struct that holds pointers to the next datapoint and the first coordinate of the current datapoint,
 *  as well as the index of the centroid in the centroid matrix to which this datapoint is closest. */
struct datapoint {
    struct datapoint* next;
    struct coordinate* pointer;
    int cluster;
};

typedef struct coordinate coordinate;
typedef struct datapoint datapoint;

int k;  /* The number of desired centroids. */
int num_of_points;  /* The number of given datapoints. */
int max_iter;   /* The maximal number of Kmeans iterations. */
int dimension;  /* The number of coordinates in each datapoint and centroid. */
double EPSILON; /* The value of convergence. */
double** centroids; /* An array of arrays of double-type-objects that represents the current (old) centroids. */
double** new_centroids; /* An array of arrays of double-type-objects that represents the new centroids. */
int points_allocated;  /* To be deleted */

void general_error();
void free_coordinates(coordinate*);
void free_datapoints(datapoint*);
void free_centroids(double**, int);
void free_arrays(double**);
void free_memory(datapoint*, int);
double* PyToC_Points(PyObject*, double**, char);
double** PyToC_PointsArray(PyObject*, int, char);
coordinate* pointToCoors(double*, double**, datapoint*, datapoint*);
datapoint* arrayToData(double**);
void allocateNewCens(datapoint*);
datapoint* PyToC(PyObject*, PyObject*);
double distance(datapoint*, double*);
void closest_cluster(datapoint*);
void update_centroid(datapoint*);
double distance_doubles(double*, double*);
int delta_centroid();
void swapCentroids(double***, double***);
char** round_centroid(double*, datapoint*);
char*** round_all(datapoint*);
char*** Kmeans(datapoint*);
PyObject* createCentroid(char**);
PyObject* createCentroidArray(char***);

/** A method that prints an error in Python and exits the program.
 *  Takes no arguments. */
void general_error() {
    /* Below are 2 other ways to implement the printing of the error message via Python.
    The first would print it as a Value Error, the other would print it to the error data
    stream. We chose to print it as a Python print method usually does, and terminate both
    programs thorugh this mehtod. */
    /* Method 1: */
    /* PyErr_SetString(PyExc_ValueError, "An Error Has Occurred\n"); */
    /* Method 2: */
    /* PyErr_SetString(PyExc_SystemExit, "An Error Has Occurred\n"); */
    PySys_WriteStdout("An Error Has Occurred\n");
    Py_Exit(1); /* Isn't neccesary if we used method 2. */
    exit(1);
}

/** A method that frees the memory allocated for a coordinate-struct.
 *  @param coor
 *      A pointer to a coordinate-type-object, which marks the first coordinate in the struct. */
void free_coordinates(coordinate* coor) {
    coordinate* tmp;
    while (coor->next != NULL) {
        tmp = coor;
        coor = coor->next;
        free(tmp);
    }
}

/** A method that frees the memory allocated for a datapoint-struct.
 *  @param point
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct. */
void free_datapoints(datapoint* point) {
    datapoint* tmp;
    while (point != NULL) {
        tmp = point;
        point = point->next;
        free_coordinates(tmp->pointer);
        free(tmp);
    }
}

/** A method that frees the memory allocation for all the arrays of double-type-objects in a
 *  given array of arrays of double-type-objects that's meant to represent centroids (either
 *  new or old).
 *  @param array
 *      The array of arrays of double-type-objects.
 *  @param size
 *      The size of the given array of arrays of double-type-objects.   */
void free_centroids(double** array, int size) {
    int i;
    for (i = 0; i < size; i++) { free(array[i]); }
    free(array);
}

/** A method that frees the memory allocated for the centroids array of arrays of double-type-objects,
 *  and the given array of arrays of double-type-objects, and prints the general error message.
 *  @param datapoints
 *      The given array of arrays of double-type-objects.   */
void free_arrays(double** datapoints) {
    free_centroids(datapoints, num_of_points);
    free_centroids(centroids, k);
    general_error();
}

/** A method that frees the memory allocation for all the datapoint-type-objects, as well
 *  as the memory allocated for both 'centroids' and 'new_centroids', and all their elements.
 *  Prints the general error message.
 *  @param head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct.
 *  @param all
 *      An integer meant to denote if the method is to free the memory for both the old and the
 *      new centroids arrays, or just the old centroids array, where: 0 = free just the old centroids
 *      array. 1 = free both the old and the new centroids array.  */
void free_memory(datapoint* head, int all) {
    free_datapoints(head);
    free_centroids(centroids, k);
    if (all == 1) { free_centroids(new_centroids, sizeof(new_centroids)); }
    general_error();
}

/** A method that converts a PyObject-type-object into an array of double-type-objects.
 *  @param py_point
 *      A pointer to the PyObject-type-object.
 *  @param py_points
 *      A pointer to the array of PyObject-type-objects where pt_point was found.
 *  @param point_type
 *      The type of the point that we're creating, where 'c' is for a centroid and 'd' is for a datapoint.
 *  @return
 *      The array of double-type-objects (could represent either a centroid or a datapoint as an array of
 *      double-type-objects instead of a datapoint-type-object). */
double* PyToC_Points(PyObject* py_point, double** points, char point_type) {
    int i;
    double* point;
    point = malloc(dimension * sizeof(double));
    if (point == NULL) {    /* Validating that the memory allocation was successful */
        if (point_type == 'd') { free_centroids(centroids, k); }
        free_centroids(points, sizeof(points));
        general_error();
    }
    for (i = 0; i < dimension; i++) { point[i] = PyFloat_AsDouble(PyList_GetItem(py_point, i)); }
    return point;
}

/** A method that converts a PyObject-type-object into an array of arrays of double-type-objects,
 *  which will represent either all centroids or all datapoints, as given by Python.
 *  @param py_points
 *      A pointer to the PyObject-type-object.
 *  @param point_num
 *      The number of centroids or datapoints that should be converted.
 *  @param point_type
 *      The type of the points that we're creating, where 'c' is for centroids and 'd' is for datapoints.
 *  @return
 *      The array of arrays of double-type-objects. */
double** PyToC_PointsArray(PyObject* py_points, int point_num, char point_type) {
    int i;
    double** points;
    points = malloc(point_num * sizeof(double*));
    if (points == NULL) {   /* Validating that the memory allocation was successful */
        if (point_type == 'd') { free_centroids(centroids, k); }
        general_error();
    }
    for (i = 0; i < point_num; i++) {
        points[i] = PyToC_Points(PyList_GetItem(py_points, i), points, point_type);
    }
    return points;
}

/** A method that converts an array of double-type-objects into a coordinate-type-object struct.
 *  @param datapoint
 *      The array of double-type-objects.
 *  @param datapoints
 *      The array of arrays of double-type-objects where datapoint was found.
 *  @param head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct.
 *  @param curr_data
 *      A pointer to a datapoint-type-object, which the method was called to create its coordinates.
 *  @return
 *      A pointer to the coordinate-type-object, which marks the first coordinate in the struct. */
coordinate* pointToCoors(double* data, double** datapoints, datapoint* head, datapoint* curr_data) {
    int i;
    coordinate* curr_coor;
    coordinate* head_coor;
    head_coor = malloc(sizeof(coordinate));
    if (head_coor == NULL) { /* Validating that the memory allocation was successful */
        free(curr_data);
        if (curr_data != head) { free_datapoints(head); }
        free_arrays(datapoints);
    }
    curr_coor = head_coor;
    for (i = 0; i < (dimension - 1); i++) {
        curr_coor->value = data[i];
        curr_coor->next = malloc(sizeof(coordinate));
        if (curr_coor->next == NULL) {  /* Validating that the memory allocation was successful */
            free_datapoints(head);
            free_arrays(datapoints);
        }
        curr_coor = curr_coor->next;
        curr_coor->next = NULL;
    }
    curr_coor->value = data[i];
    free(data); /* Freeing the memory we allocated as it is no longer necessary. */
    return head_coor;
}

/** A method that converts an array of arrays of double-type-objects into a datapoint-type-object
 *  struct.
 *  @param datapoints
 *      The array of arrays of double-type-objects.
 *  @return
 *      A pointer to the datapoint-type-object, which marks the first datapoint in the struct. */
datapoint* arrayToData(double** datapoints) {
    int i;
    datapoint* curr_data;
    datapoint* head;
    head = malloc(sizeof(datapoint));
    if (head == NULL) { free_arrays(datapoints); }  /* Validating that the memory allocation was successful */
    curr_data = head;
    for (i = 0; i < (num_of_points - 1); i++) {
        curr_data->pointer = pointToCoors(datapoints[i], datapoints, head, curr_data);
        curr_data->next = malloc(sizeof(datapoint));
        if (curr_data->next == NULL) {  /* Validating that the memory allocation was successful */
            free_datapoints(head);
            free_arrays(datapoints);
        }
        curr_data = curr_data->next;
        curr_data->next = NULL;
    }
    curr_data->pointer = pointToCoors(datapoints[num_of_points - 1], datapoints, head, curr_data);
    free(datapoints); /* Freeing the memory we allocated as it is no longer necessary. */
    return head;
}

/** A method that allocates memory for the new_centroids array.
 *  @param head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct. */
void allocateNewCens(datapoint* head) {
    int i;
    new_centroids = malloc(k * sizeof(double*));
    if (new_centroids == NULL) { free_memory(head, 0); }    /* Validating that the memory allocation was successful */
    for (i = 0; i < k; i++) {
        new_centroids[i] = calloc(dimension, sizeof(double));
        if (new_centroids[i] == NULL) { free_memory(head, 1); } /* Validating that the memory allocation was successful */
    }
}

/** A method that converts all the arguments given to fit [given as PyObject-type-objects] into C objects.
 *  @param self
 *      The module object.
 *  @param args
 *      The arguments given to fit().
 *  @return
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct. */
datapoint* PyToC(PyObject* self, PyObject* args) {
    PyObject *py_centroids;
    PyObject *py_datapoints;
    datapoint* head;
    /* This parses the Python arguments into 2 PyObject (O) variables named 'py_centroids' and
        'py_datapoints', 2 int (i) variables named 'K' and 'iter', a double (d) variable named
        'eps', and another 2 int (i) variables named 'N' and 'd' : */
    if(!PyArg_ParseTuple(args, "OOiidii", &py_centroids, &py_datapoints, &k, &max_iter, &EPSILON, &num_of_points, &dimension)) {
        general_error(); }
    centroids = PyToC_PointsArray(py_centroids, k, 'c');    
    head = arrayToData(PyToC_PointsArray(py_datapoints, num_of_points, 'd'));
    allocateNewCens(head);
    return head;
}

/** A method that calculates the Euclidean Distance between a datapoint-type object and an array of
 *  double-type-objects (which represents a centroid).
 *  @param p1
 *      A pointer to the datapoint-type-object.
 *  @param p2
 *      The array of double-type-objects, which represents a centroid.
 *  @return
 *      The Euclidean Distance between p1 and p2 as a double-type-object.
 * 
 *  For more information about Euclidean Distance, see:
 *  https://en.wikipedia.org/wiki/Euclidean_distance    */
double distance(datapoint* p1, double* p2) {
    int i;
    double sum;
    coordinate* curr_cord;
    i = 0;
    sum = 0.0;
    curr_cord = p1->pointer;
    while (curr_cord != NULL) {
        sum += pow((curr_cord->value - p2[i]), 2);
        i++;
        curr_cord = curr_cord->next;
    }
    return sqrt(sum);
}

/** A method that updates the cluster field of a datapoint-type-object, to become the index of the
 *  closest centroid to said datapoint-type-object (in relation to Euclidean Distance) within the
 *  centroids array of double-type-objects.
 *  @param point_head
 *      A pointer to the datapoint-type-object.
 * 
 *  For more information about Euclidean Distance, see:
 *  https://en.wikipedia.org/wiki/Euclidean_distance    */
void closest_cluster(datapoint* point_head) {
    int min_index;
    int i;
    double min_distance;
    double curr_distance;
    min_distance = distance(point_head, centroids[0]);
    min_index = 0;
    for (i = 1; i < k; i++) {
        curr_distance = distance(point_head, centroids[i]);
        if (curr_distance <= min_distance) {
            min_distance = curr_distance;
            min_index = i;
        }
    }
    point_head->cluster = min_index;
    points_allocated++;
}

/** A method that updates the new_centroids array of double-type-objects.
 *  @param point_head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct. */
void update_centroid(datapoint* point_head) {
    int i;
    int j;
    int point_counter;
    double* sums;
    coordinate* curr_coordinate;
    datapoint* curr_point;
    point_counter = 0;
    curr_point = point_head;
    sums = calloc(dimension, sizeof(double*));
    if (sums == NULL) { free_memory(point_head, 1); }   /* Validating that the memory allocation was successful */
    for (i = 0; i < k; i++) {
        while (curr_point != NULL) {
            if (curr_point->cluster == i) {
                curr_coordinate = curr_point->pointer;
                point_counter++;
                for (j = 0; j < dimension; j++) {
                    sums[j] += curr_coordinate->value;
                    curr_coordinate = curr_coordinate->next;
                }
            }
            curr_point = curr_point->next;
        }
        for (j = 0; j < dimension; j++) {
            new_centroids[i][j] = sums[j] / point_counter;
            sums[j] = 0.0;  /* Setting sum[j] back to 0.0 for the next iteration of i */
        }
        curr_point = point_head;
        point_counter = 0;
    }
    free(sums); /* Freeing the memory we allocated as it is no longer necessary. */
}

/** A method that calculates the Euclidean Distance between 2 arrays of double-type-objects (which
 *  represent an old and a new centroid).
 *  @param p1
 *      An array of double-type-objects that represents an old centroid.
 *  @param p2
 *      An array of double-type-objects that represents a new centroid.
 *  @return
 *      The Euclidean Distance between p1 and p2 as a double-type-object.
 * 
 *  For more information about Euclidean Distance, see:
 *  https://en.wikipedia.org/wiki/Euclidean_distance    */
double distance_doubles(double* p1, double* p2) {
    double sum;
    int i;
    sum = 0.0;
    for (i = 0; i < dimension; i++) { sum += pow((p1[i] - p2[i]), 2); }
    return sqrt(sum);
}

/** A method that returns whether the distance (in relation to Euclidean Distance) between every old
 *  centroid to its respective new centroid is smaller than EPSILON.
 *  @return
 *      An integer that is either 0 or 1, where:
 *          0 means that there's at least a single pair of such centroids so that the distance
 *              between them is greater than, or equal to, EPSILON.
 *          1 means that the distance between every pair of such centroids is smaller than EPSILON.
 * 
 *  For more information about Euclidean Distance, see:
 *  https://en.wikipedia.org/wiki/Euclidean_distance    */
int delta_centroid() {
    int i;
    for (i = 0; i < k; i++) {
        if (distance_doubles(centroids[i], new_centroids[i]) >= EPSILON) { return 0; } }
    return 1;
}

/** A method that swaps between 2 arrays of double-type-objects (using pointers).
 *  @param old_centroids
 *      A pointer to an array of double-type-objects.
 *  @param centroids_new
 *      A pointer to an array of double-type-objects.   */
void swapCentroids(double*** old_centroids, double*** centroids_new) {
    double** tmp;
    tmp = *old_centroids;
    *old_centroids = *centroids_new;
    *centroids_new = tmp;
}
 
 /** A method that rounds every double-type-object in a given array to show 4 digits after the decimal
  *  point.
  *  @param centroid
  *         The array.
  *  @param head
  *         A pointer to a datapoint-type-object, which marks the first datapoint in the struct.
  *  @return
  *         An array of strings, each is an array of char-type-objects that represents a rounded coordinate
  *         of the given centroid.   */
char** round_centroid(double* centroid, datapoint* head) {
    int i;
    int size;
    char** res;
    res = malloc(dimension * sizeof(char*));
    if (res == NULL) { free_memory(head, 1); }  /* Validating that the memory allocation was successful */
    for (i = 0; i < dimension; i++) {
        size = snprintf(NULL, 0, "%.4f", centroid[i]) + 1;
        res[i] = malloc(size * sizeof(char));
        if (res[i] == NULL) { free_memory(head, 1); }  /* Validating that the memory allocation was successful */
        snprintf(res[i], size, "%.4f", centroid[i]);
    }
    return res;
}

/** A method that rounds all the coordinates of every centroid to show 4 digits after the decimal
 *  point.
 *  @param head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct.
 *  @return
 *      An array of arrays, each is an array of strings that represents a centroid after rounding all
 *      its coordinates.   */
char*** round_all(datapoint* head) {
    int i;
    char*** res;
    res = malloc(k * sizeof(char**));
    if (res == NULL) { free_memory(head, 1); }  /* Validating that the memory allocation was successful */
    for (i = 0; i < k; i++) { 
        res[i] = round_centroid(centroids[i], head); }
    return res;
}

/** A method that does the Kmeans algorithm, and updates the centroids array in-place. It also frees the
 *  memory allocated for the datapoint-struct.
 *  @param head
 *      A pointer to a datapoint-type-object, which marks the first datapoint in the struct.
 *  @return
 *      The final centroids array of arrays of double-type-objects.
 * 
 *  For more information about the Kmeans algorithm, see:
 *  https://en.wikipedia.org/wiki/K-means_clustering    */
char*** Kmeans(datapoint* head) {
    int num_iteration;
    datapoint* curr_point;
    num_iteration = 0;
    curr_point = head;
    while (num_iteration < max_iter) {
        points_allocated = 0;
        while (curr_point != NULL) {
            closest_cluster(curr_point);
            curr_point = curr_point->next;
        }
        update_centroid(head);
        if (delta_centroid() == 1) {    /* The update converges as requested */
            swapCentroids(&centroids, &new_centroids);
            break;
        }
        swapCentroids(&centroids, &new_centroids);
        num_iteration++;
        curr_point = head;
    }
    free_datapoints(head);
    return round_all(head); /* Rounding all the centroids as specified in the PDF file */
}

/** A method that converts an array of arrays of char-type-objects (which represent a centroid after
 *  all of its coordinates have been rounded) into a PyObject-type-object that represents.
 *  @param coordinates
 *      The array of arrays of char-type-objects.
 *  @return
 *      A pointer to the PyObject-type-object.  */
PyObject* createCentroid(char** coordinates) {
    int i;
    PyObject* centroid;
    centroid = PyList_New(dimension);
    for (i = 0; i < dimension; i++) {
        PyList_SET_ITEM(centroid, i, PyUnicode_FromString(coordinates[i]));
        free(coordinates[i]);
    }
    free(coordinates);
    return centroid;
}

/** A method that converts the array that represents the centroids after rounding all of their
 *  coordinates into a PyObject-type-object.
 *  @param centroids_C
 *      An array of arrays of arrays of char-type-objects that represents all the centroids after
 *      rounding their coordinates.
 *  @return
 *      A pointer to the PyObject-type-object.  */
PyObject* createCentroidArray(char*** centroids_C) {
    PyObject* centroid_array;
    int i;
    centroid_array = PyList_New(k);
    for (i = 0; i < k; i++) { PyList_SET_ITEM(centroid_array, i, createCentroid(centroids_C[i])); }
    free(centroids_C);  /* By now all the memory allocation for each element in centroids_C has been freed */
    return centroid_array;
}

static PyObject* fit(PyObject* self, PyObject* args) {
    datapoint* head;
    head = PyToC(self, args);
    return createCentroidArray(Kmeans(head));
}

static PyMethodDef KmeansMethods[] = {
    {"fit",                 /* the Python method name that will be used */
      (PyCFunction) fit,    /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,         /* flags indicating parameters accepted for this function */
      PyDoc_STR("Compute the K-means algorithm in C with arguments from Python, each validated in advance.\n\nThe argument declarations refer to Python object types.\n\nArgs:\n    clusters (List[List[float]]):\n        The initial K-means clusters (or centroids).\n    datapoints (List[List[float]]):\n        The K-means datapoints.\n    K (int):\n        The number of desired K-means clusters (or centroids). Must be between 2 and 'N - 1' (inclusive).\n    iter (int):\n        The maximal number of desired K-means iterations. Must be between 2 and 999 (inclusive).\n    eps (float):\n        The epsilon value for K-means convergence. Must be non-negative.\n    N (int):\n        The number of datapoints. Must be non-negative!\n    d (int):\n        The number of observations (or coordinates) in each datapoint and cluster (or centroid).\n\nReturns:\n    List[List[str]]:\n        A list of the final K-means clusters (or centroids), where:\n            - Each cluster (or centroid) is represented by a list, where:\n            - Each element is a string representing an observation (or a coordinate), so that:\n                Each such string shows the data after being rounded to show only 4 digits past the decimal point.\n\nFor more information about the K-means algorithm, see:\n    https://en.wikipedia.org/wiki/K-means_clustering")},
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    KmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) { general_error(); }
    return m;
}
