#pragma once
#include <stdlib.h>
#include<stdio.h>

typedef struct matrix {
	short size;
	long det;
	int **data;
} matrix;

void free_matrix(matrix* M);
void print_matrix(FILE *file, matrix *matrix);
void free_matrixes(matrix* matrixes, int number);
matrix* allocate_matrix(int size);
matrix *additional_matrix(matrix *M, matrix *result, int row, int column);
long det(matrix *M);
matrix *read_matrixes(FILE *input, int size);
int greater_det(void* one, void* two);
void print_matrixes(FILE *file, matrix *matrixes, int number);
matrix *count_matrixes_dets(matrix *matrixes, int num);
