#ifndef SYMNMF_H
#define SYMNMF_H
#include <stdio.h>
#include <stdlib.h>


extern int N;   /* The number of given points */
extern int dim; /* The shared dimension of all points */
extern int K;   /* The desired number of symnmf clusters (or centroids) */
extern double **points_array;   /* The given points */
extern double **A;  /* The similarity matrix */
extern double **D;  /* The diagonal degree matrix */
extern double **W;  /* The normalized similarity matrix */
extern double **H;  /* The decomposition matrix */

void freeArrayDouble(double **array, int array_len);
void freeAll();
int allocateMatDouble(double ***mat, int rows);
int allocateRowDouble(double **row, int len, int alloc);
int sym();
int ddg();
int norm();
int symnmf();


#endif /* SYMNMF_H */