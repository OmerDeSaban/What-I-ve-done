#define  _GNU_SOURCE
#include "symnmf.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int N;  /* The number of given points */
int dim;    /* The shared dimension of all points */
int K;  /* The desired number of symnmf clusters (or centroids) */
double **points_array;  /* The given points */
double **A; /* The similarity matrix */
double **D; /* The diagonal degree matrix */
double **D_ISR; /* The Inverted Square-Root matrix of the diagonal degree matrix. */
double **tmp_mat;   /* A temporary matrix for the calculation of either W or H's updates */
double **W; /* The normalized similarity matrix */
double **H; /* The decomposition matrix */
double **transH;    /* The decomposition matrix's transposed matrix */
double **newH;  /* The newest generated version of the decomposition matrix */
double **numerator; /* The numerator matrix in the calcualtion of H's newest version */
double **denominator;   /* The denominator matrix in the calculation of H's newest version */
double **Sub;   /* The matrix resulting from subtracting the previous version of H from its newest 1 */


void generalError();
int matError();
int rowError(double **, int);
double euclideanDistanceSquare(double *, double *);
double expCalc(double *, double *);
int invertedSquareRootMat();
int mulMat(double **, double **, int, double **, int, int);
int H_mem();
int calcUpdate();
double squaredFrobeniusNorm();
FILE *openFile(char *);
int readFile(char *);
void printResult(double **);


/** Frees all remaining allocated memory, prints the general error message "An Error Has Occurred" via C
 *  and terminates the program.
 *  Takes no arguments. */
void generalError() {
    freeAll();
    printf("An Error Has Occurred\n");
    exit(1);
}

/** Frees all the memory allocated for an array of arrays of doubles.
 *  @param array The array whose memory is to be freed.
 *  @param array_len The length of the given array. */
void freeArrayDouble(double **array, int array_len) {
    int i;
    if (array == NULL) { return; }
    if (array_len != 0) {
        for (i = 0; i < array_len; i++) {
            free(array[i]);
            array[i] = NULL;
        }
    }
    free(array);
}

/** Frees all remaining dynamically allocated memory.
 *  Takes no arguments. */
void freeAll() {
    if (points_array != NULL) { freeArrayDouble(points_array, N); }
    points_array = NULL;
    if (A != NULL) { freeArrayDouble(A, N); }
    A = NULL;
    if (D != NULL) { freeArrayDouble(D, N); }
    D = NULL;
    if (D_ISR != NULL) { freeArrayDouble(D_ISR, N); }
    D_ISR = NULL;
    if (tmp_mat != NULL) { freeArrayDouble(tmp_mat, N); }
    tmp_mat = NULL;
    if (W != NULL) { freeArrayDouble(W, N); }
        W = NULL;
    if (H != NULL) {freeArrayDouble(H, N); }
    H = NULL;
    if (transH != NULL) { freeArrayDouble(transH, K); }
    transH = NULL;
    if (newH != NULL) { freeArrayDouble(newH, N); }
    newH = NULL;
    if (numerator != NULL) { freeArrayDouble(numerator, N); }
    numerator = NULL;
    if (denominator != NULL) { freeArrayDouble(denominator, N); }
    denominator = NULL;
    if (Sub != NULL) { freeArrayDouble(Sub, N); }
    Sub = NULL;
}

/** Frees all remaining dynamically allocated memory in a case of a matrix's memory allcation error.
 *  Takes no arguments.
 *  @return An integer, 1, denoting that an error has occured.  */
int matError() {
    freeAll();
    return 1;
}

/** Frees all remaining dynamically allocated memory in a case of a matrix's row's memory allcation
 *  error.
 *  @param mat The matrix whose row's memory allocation failed.
 *  @param len The number of rows of mat whose memory has already been allocated. should be non-negative.
 *  @return An integer, 1, denoting that an error has occured.  */
int rowError(double **mat, int len) {
    freeArrayDouble(mat, len);
    return matError();
}

/** Allocates memory for a new matrix of doubles, represented by an array of arrays of doubles.
 *  @param mat A pointer to the global matrix whose memory we wish to allocate.
 *  @param rows The number of rows this matrix should have.
 *  @return An integer denoting if the memory allocation failed, where:
 *      1 means that it has failed ;
 *      0 means it was successful.  */
int allocateMatDouble(double ***mat, int rows) {
    int i;
    *mat = (double **) malloc (rows * sizeof(double*));
    if (*mat == NULL) { return 1; }    /* Memory allocation error */
    for (i = 0; i < rows; i++) { (*mat)[i] = NULL; }
    return 0;
}

/** Allocates, or initializes, memory for a new row of doubles, represented by an array of doubles.
 *  @param row A pointer to the row whose memory we wish to allocate.
 *  @param len The length of the desired row.
 *  @param alloc An integer that denotes whether this function should only allocate the memory, or also
 *      initalize it, where:
 *      1 means that the function should only allocate the memory ;
 *      Any other integer means that it should initialize it.
 *  @return An integer denoting if the memory allocation failed, where:
 *      1 means that it has failed ;
 *      0 means it was successful.  */
int allocateRowDouble(double **row, int len, int alloc) {
    if (alloc == 1) { *row = (double *) malloc(len * sizeof(double)); }
    else { *row = (double *) calloc(len, sizeof(double)); }
    if (*row == NULL) { return 1; }  /* Memory allocation error */
    return 0;
}

/** Calculates the squared Euclidean Distance between 2 given points (i-th and j-th points).
 *  @param point_i The i-th point.
 *  @param point_j The j-th point.
 *  @return The squared Euclidean Distance between the 2 given points.
 * 
 *  For more information about Euclidean Distance, see:
 *  https://en.wikipedia.org/wiki/Euclidean_distance.   */
double euclideanDistanceSquare(double *point_i, double *point_j) {
    int i;
    double dist;
    dist = 0.0;
    for (i = 0; i < dim; i++) { dist += pow((point_i[i] - point_j[i]), 2); }
    return dist;
}

/** Calculates the exponenet of e, as needed for the j-th element of the i-row of the similarity matrix.
 *  @param point_i The i-th point.
 *  @param point_j The j-th point.
 *  @return The calculated exponent.    */
double expCalc(double *point_i, double *point_j) {
    double dist;
    dist = euclideanDistanceSquare(point_i, point_j);
    return -dist / 2;
}

/** Calculates the similarity matrix using the given points.
 *  Updates the global A.
 *  Takes no arguments.
 *  @return An integer denoting if the function succeeded, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int sym() {
    int i, j;
    A = NULL;
    if (allocateMatDouble(&A, N) == 1) { return matError(); }   /* Memory allocation error */
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&A[i], N, 0) == 1) { return rowError(A, i); } /* Memory allocation error */
        for (j = 0; j < N; j++) {
            if (i != j) { A[i][j] = exp(expCalc(points_array[i], points_array[j])); }
        }
    }
    return 0;
}

/** Calculates the diagonal degree matrix using the given points.
 *  Updates the global D.
 *  Takes no arguments.
 *  @return An integer denoting if the function succeeded, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int ddg() {
    int i, j;
    if (sym() == 1) { return 1; }   /* Algorithm error */
    D = NULL;
    if (allocateMatDouble(&D, N) == 1) { return matError(); }   /* Memory allocation error */
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&D[i], N, 0) == 1) { return rowError(D, i); } /* Memory allocation error */
        D[i][i] = 0.0;
        for (j = 0; j < N; j++) { D[i][i] += A[i][j]; }
    }
    return 0;
}

/** Generates the inverted square-root matrix of the diagonal degree matrix.
 *  Updates the global D_ISR.
 *  Takes no arguments.
 *  @return An integer denoting if the function succeeded, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int invertedSquareRootMat() {
    int i;
    if (ddg() == 1) { return 1; }   /* Algorithm error */
    if (allocateMatDouble(&D_ISR, N) == 1) { return matError(); }   /* Memory allocation error */
    for (i = 0; i < N; i++) {
        if (D[i][i] != 0) {
            if (allocateRowDouble(&D_ISR[i], N ,0) == 1) { return rowError(D_ISR, i); } /* Memory allocation error */
            D_ISR[i][i] = (1 / sqrt(D[i][i]));
        }
        else { return rowError(D_ISR, i); } /* (D[i][i] == 0), division by 0 error */
    }
    return 0;
}

/** Calculates, and updates in-place the matrix resulting from multiplying the 2 given matrices.
 *  @param mul_mat The matrix that should represent the multiplication result.
 *  @param matrix1 A matrix. Should have as many columns as the number of rows in matrix2.
 *  @param mat1_rows The number of rows in matrix1.
 *  @param matrix2 A matrix. Should have as many rows as the number of columns in matrix1.
 *  @param mat2_columns The number of columns in matrix2.
 *  @param shared_dim The shared number of columns in matrix1 and rows in matrix2.
 *  @return An integer denoting if the multiplication was successful, where:
 *      1 means that there was an error ;
 *      0 means that it was successful. */
int mulMat(double **mul_mat, double **matrix1, int mat1_rows, double **matrix2, int mat2_columns, int shared_dim) {
    int i, j, u;
    double elem_ij;
    if ((!mul_mat) || (!matrix1) || (!matrix2)) { return 1; } /* Null pointer error */
    for (i = 0; i < mat1_rows; i++) {
        if (!matrix1[i]) { return 1; }  /* Null pointer error */
        for (j = 0; j < mat2_columns; j++) {
            elem_ij = 0.0;
            for (u = 0; u < shared_dim; u++) {
                if (!matrix2[u]) { return 1; }  /* Null pointer error */
                elem_ij += (matrix1[i][u] * matrix2[u][j]);
            }
            mul_mat[i][j] = elem_ij;
        }
    }
    return 0;
}

/** Calculates the normalized similarity matrix using the given points.
 *  Updates the global W.
 *  Takes no arguments.
 *  @return An integer denoting if the function succeeded, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int norm() {
    int i;
    D_ISR = NULL;
    tmp_mat = NULL;
    W = NULL;
    if (invertedSquareRootMat() == 1) { return 1; } /* Algorithm error */
    if (allocateMatDouble(&tmp_mat, N) == 1) { return matError(); } /* Memory allocation error */
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&tmp_mat[i], N, 0) == 1) { return rowError(tmp_mat, i); } /* Memory allocation error */
    }
    if (allocateMatDouble(&W, N) == 1) { return matError(); }   /* Memory allocation error */
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&W[i], N, 0) == 1) { return rowError(W, i); } /* Memory allocation error */
    }
    if (mulMat(tmp_mat, D_ISR, N, A, N, N) == 1) { return 1; }  /* Algorithm error */
    if (mulMat(W, tmp_mat, N, D_ISR, N, N) == 1) { return 1; }  /* Algorithm error */
    return 0;
}

/** Allocates all memory needed for the matrices needed to calculate H's updates.
 *  Updates the global tmp_mat, transH, newH, numerator, denominator, and Sub.
 *  Takes no arguments. All possible errors here are memory allocation errors.
 *  @return An integer denoting if all memory allocation was successful, where:
 *      1 means that an error has occurred ;
 *      0 means that all memory allocation was successful.  */
int H_mem() {
    int i;
    tmp_mat = NULL;
    if (allocateMatDouble(&tmp_mat, N) == 1) { return matError(); }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&tmp_mat[i], N, 1) == 1) { return rowError(tmp_mat, i); }
    }
    transH = NULL;
    if (allocateMatDouble(&transH, K) == 1) { return matError(); }
    for (i = 0; i < K; i++) {
        if (allocateRowDouble(&transH[i], N, 1) == 1) { return rowError(transH, i); }
    }
    newH = NULL;
    if (allocateMatDouble(&newH, N) == 1) { return matError(); }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&newH[i], K, 1) == 1) { return rowError(newH, i); }
    }
    numerator = NULL;
    if (allocateMatDouble(&numerator, N) == 1) { return matError(); }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&numerator[i], K, 1) == 1) { return rowError(numerator, i); }
    }
    denominator = NULL;
    if (allocateMatDouble(&denominator, N) == 1) { return matError(); }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&denominator[i], K, 1) == 1) { return rowError(denominator, i); }
    }
    Sub = NULL;
    if (allocateMatDouble(&Sub, N) == 1) { return matError(); }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&Sub[i], K, 1) == 1) { return rowError(Sub, i); }
    }
    return 0;
}

/** Updates the global transH, numerator, denominator, newH, Sub, and H (in this order) to be in respect
 *  to H's latest update.
 *  Takes no arguments.
 *  @return An integer denoting if the update was successful, where:
 *      1 means that there was an error ;
 *      0 means that the update was successful. */
int calcUpdate() {
    int i, j;
    /* Update transH : */
    for (i = 0; i < K; i++) { for (j = 0; j < N; j++) { transH[i][j] = H[j][i]; } }
    /* Update numerator : */
    if (mulMat(numerator, W, N, H, K, N) == 1) { return 1; }    /* Algorithm error */
    /* Update denominator : */
    if (mulMat(tmp_mat, H, N, transH, N, K) == 1) { return 1; } /* Algorithm error */
    if (mulMat(denominator, tmp_mat, N, H, K, N) == 1) { return 1; }    /* Algorithm error */
    /* Update newH : */
    for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) { newH[i][j] = (H[i][j] * (0.5 + (0.5 * (numerator[i][j] / denominator[i][j])))); }
    }
    /* Update Sub : */
    for (i = 0; i < N; i++) { for (j = 0; j < K; j++) { Sub[i][j] = (newH[i][j] - H[i][j]); } }
    /* Update H : */
    for (i = 0; i < N; i++) { for (j = 0; j < K; j++) { H[i][j] = newH[i][j]; } }
    return 0;
}

/** Calculates the squared Frobenius Norm of the global Sub.
 *  Takes no arguments.
 *  @return The squared Frobenius Norm.
 * 
 *  For more information about Forbenius Norm, see:
 *  https://en.wikipedia.org/wiki/Matrix_norm#Frobenius_norm.   */
double squaredFrobeniusNorm() {
    int i, j;
    double res;
    res = 0.0;
    for (i = 0; i < N; i++) { for (j = 0; j < K; j++) { res += pow(Sub[i][j], 2); } }
    return res;
}

/** Calculates the decomposition matrix using the given points.
 *  Updates the global H.
 *  Takes no arguments.
 *  @return An integer denoting if the function succeeded, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int symnmf() {
    int i;
    if (H_mem() == 1) { return 1; } /* Memory allocation error */
    for (i = 0; i < 300; i++) {
        if (calcUpdate() == 1) { return 1; }    /* Algorithm error */
        if (squaredFrobeniusNorm() < 0.0001) { break; }
    }
    return 0;
}

/** Opens the file given as a user CMD argument, and gets the number of points from it.
 *  Updates the global N to be the number of points in the given file. Also allocates
 *  memory for the global points_array.
 *  @param file_name The name of the .txt file where the points are stored.
 *  @return The opened file, or NULL if an error occurred.  */
FILE *openFile(char *file_name) {
    int i;
    double val;
    char a_char;
    FILE *file;
    file = fopen(file_name, "r");
    if (!file) { return NULL; } /* Error opening the file */
    dim = 0;
    while (fscanf(file, "%lf%c", &val, &a_char) == 2) {
        dim++;
        if (a_char == '\n') { break; }
    }
    N = 1;
    while (fscanf(file, "%lf%c", &val, &a_char) == 2) { if (a_char == '\n') { N++; } }
    if (N == 0) { return NULL; }    /* Invalid number of points in the given file */
    rewind(file);
    if (allocateMatDouble(&points_array, N) == 1) {  /* Memory allocation error */
        fclose(file);
        return NULL;
    }
    for (i = 0; i < N; i++) {
        if (allocateRowDouble(&points_array[i], dim, 1) == 1) { /* Memory allocation error */
            freeArrayDouble(points_array, i);
            fclose(file);
            return NULL;
        }
    }
    return file;
}

/** Reads the file given as a user CMD argument and gets the points from it.
 *  Updates the global points_array to be the read points.
 *  @param file_name The name of the .txt file where the points are stored.
 *  @return An integer denoting if this function failed, where:
 *      1 means that it has failed ;
 *      0 means that it was successful. */
int readFile(char *file_name) {
    int i, j, len;
    double val;
    char a_char;
    FILE *file;
    len = (int)strlen(file_name);
    if ((len < 4) || (strcmp(&file_name[len-4], ".txt") != 0)) { return 1;} /* Invalid file */
    file = openFile(file_name);
    if (file == NULL) { return 1; } /* Error opening the file */
    i = 0;
    j = 0;
    while (fscanf(file, "%lf%c", &val, &a_char) == 2) {
        points_array[i][j] = val;
        if (a_char == '\n') {
            i++;
            j = 0;
        } else {
            j++;
        }
    }
    fclose(file);
    return 0;
}

/** Prints the result matrix, after rounding all its elements to have 4 digits after the decimal point.
 *  @param matrix The result matrix.    */
void printResult(double **matrix) {
    int i;
    int j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (j == (N - 1)) { printf("%.4f\n", matrix[i][j]); }
            else { printf("%.4f,", matrix[i][j]); }
        }
    }
}

int main(int argc, char *argv[]) {
    const char *goal;
    if (argc != 3) { generalError(); }  /* Too many or not enough CMD arguments */
    goal = argv[1];
    /* Validating we got a viable goal : */
    if ((strcmp(goal, "sym") != 0) && (strcmp(goal, "ddg") != 0) && (strcmp(goal, "norm") != 0)) { generalError(); }
    points_array = NULL;
    if (readFile(argv[2]) == 1) { generalError(); } /* Error reading the file */
    if (strcmp(goal, "sym") == 0) {
        if (sym() == 1) { generalError(); } /* Algorithm error */
        printResult(A);
    }
    else if (strcmp(goal, "ddg") == 0) {
        if (strcmp(goal, "ddg") == 0) {
            if (ddg() == 1) { generalError(); } /* Algorithm error */
            printResult(D);
        }
    }
    else {  /* goal is "norm" */
        if (norm() == 1) { generalError(); }    /* Algorithm error */
        printResult(W);
    }
    freeAll(); /* Free all allocated memory */
    return 0;
}
