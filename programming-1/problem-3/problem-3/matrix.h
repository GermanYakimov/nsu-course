#pragma once
#include <stdlib.h>
#include<stdio.h>

typedef struct matrix {
	size_t size;
	double det;
	double **data;
} matrix;

void free_matrix(matrix* M);
void print_matrix(FILE *file, matrix *matrix);
void free_matrixes(matrix* matrixes, size_t number);
matrix* allocate_matrix(size_t size);
matrix *additional_matrix(matrix *M, matrix *result, int row, int column);
double det(matrix *M);
double det_quick(matrix *M);
matrix *read_matrixes(FILE *input, size_t size);
double greater_det(void* one, void* two);
void print_matrixes(FILE *file, matrix *matrixes, size_t number);
matrix *count_matrixes_dets(matrix *matrixes, size_t num);
