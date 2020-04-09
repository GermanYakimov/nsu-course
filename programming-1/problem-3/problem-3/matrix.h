#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "sorts.h"

typedef struct matrix {
	size_t size;
	double det;
	double **data;
} matrix;

void free_matrix(matrix* M);
void print_matrix(FILE *file, matrix *matrix, _Bool with_det);
void free_matrixes(matrix* matrixes, size_t number);
matrix* allocate_matrix(size_t size);
matrix *additional_matrix(matrix *M, matrix *result, int row, int column);
double det(matrix *M);
double det_quick(matrix *M);
matrix *count_matrixes_dets(matrix *matrixes, size_t num);
matrix *read_matrixes(FILE *input, size_t size, size_t max_order);
void *load_matrixes(FILE *input, sort_input *data);
double greater_det(void* one, void* two);
void print_matrixes(FILE *file, matrix *matrixes, size_t number, _Bool with_dets, size_t max_order);
void run_matrixes_sort_benchmark();
