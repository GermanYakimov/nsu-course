#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix.h"
#include "swap.h"

double greater_det(void* one, void* two)
{
	matrix* one_tmp = (matrix*)one;
	matrix* two_tmp = (matrix*)two;
	return one_tmp->det - two_tmp->det;
}

matrix *read_matrixes(FILE *input, size_t size)
{
	matrix *matrixes = (matrix*)calloc(size, sizeof(matrix));

	if (!matrixes)
	{
		printf("Can't allocate memore for matrixes");
		return NULL;
	}

	matrix *tmp;

	size_t order;

	for (size_t i = 0; i < size; i++)
	{
		fscanf(input, "%lu", &order);
		tmp = allocate_matrix(order);

		if (!tmp)
		{
			free_matrixes(matrixes, size);
			return NULL;
		}

		matrixes[i] = *tmp;
		free(tmp);

		for (size_t j = 0; j < order; j++)
		{
			for (size_t k = 0; k < order; k++)
			{
				fscanf(input, "%lf", (&matrixes[i].data[j][k]));
			}
		}
	}

	return matrixes;
}

matrix *copy_matrix(matrix *M)
{
	matrix *result = allocate_matrix(M->size);

	if (!result)
	{
		printf("Can't copy matrix");
		return NULL;
	}
	
	for (size_t i = 0; i < M->size; i++)
	{
		for (size_t j = 0; j < M->size; j++)
		{
			result->data[i][j] = M->data[i][j];
		}
	}

	return result;
}

matrix *count_matrixes_dets(matrix *matrixes, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		matrixes[i].det = det_quick(matrixes + i);
	}

	return matrixes;
}

double det(matrix *M)
{
	
	if (M->size == 1)
	{
		return M->data[0][0];
	}

	if (M->size == 2)
	{
		return M->data[0][0] * M->data[1][1] - M->data[1][0] * M->data[0][1];
	}

	double result = 0;

	matrix *tmp_matrix = allocate_matrix(M->size - 1);

	if (!tmp_matrix)
	{
		printf("Can't allocate memory for additional matrix");
		return LONG_MAX;
	}

	for (size_t i = 0; i < M->size; i++)
	{
		result = result + (-1)*(-1 + 2 * (i % 2)) * M->data[0][i] * det(additional_matrix(M, tmp_matrix, 0, i));
	}

	free_matrix(tmp_matrix);
	free(tmp_matrix);

	return result;
}

double det_quick(matrix *M)
{
	if (M->size == 1)
	{
		return M->data[0][0];
	}

	if (M->size == 2)
	{
		return M->data[0][0] * M->data[1][1] - M->data[1][0] * M->data[0][1];
	}

	size_t j;
	int max_index;
	double result = 1;

	matrix* M_copy = copy_matrix(M);

	for (size_t i = 0; i < M_copy->size; i++)
	{
		j = i;
		max_index = i;

		for (size_t k = i; k < M_copy->size; k++)
		{
			if (fabs(M_copy->data[k][i]) > fabs(M_copy->data[max_index][i]))
			{
				max_index = k;
			}
		}
		if (M_copy->data[max_index][i] == 0)
		{
			return 0;
		}

		if (max_index != i)
		{
			swap(M_copy->data + i, M_copy->data + max_index, sizeof(M_copy->data[max_index]));
			result *= -1;
		}

		for (size_t k = i + 1; k < M_copy->size; k++)
		{
			M_copy->data[i][k] /= M_copy->data[i][i];
		}
		result *= M_copy->data[i][i];

		M_copy->data[i][i] = 1;

		for (size_t k = i + 1; k < M_copy->size; k++)
		{
			for (size_t s = i + 1; s < M_copy->size; s++)
			{
				M_copy->data[k][s] += M_copy->data[i][s] * (-M_copy->data[k][i]);
			}
			M_copy->data[k][i] += M_copy->data[i][i] * (-M_copy->data[k][i]);
		}
	}

	free_matrix(M_copy);

	return result;
}

matrix *additional_matrix(matrix *M, matrix *result, int row, int column)
{
	int tmp_i = 0, tmp_j = 0;

	for (size_t i = 0; i < M->size; i++)
	{
		if (i == row)
		{
			continue;
		}
		tmp_j = 0;

		for (size_t j = 0; j < M->size; j++)
		{
			if (j == column)
			{
				continue;
			}
			result->data[tmp_i][tmp_j] = M->data[i][j];
			tmp_j++;
		}

		tmp_i++;

	}

	return result;
}

matrix* allocate_matrix(size_t size)
{
	matrix *result = (matrix*)malloc(sizeof(matrix));
	if (!result)
	{
		printf("Can't alocate memory for matrix");
		return NULL;
	}

	result->size = size;

	result->data = (double**)calloc(size, sizeof(double*));
	if (!result->data)
	{
		printf("Can't allocate memory for matrix");
		return NULL;
	}

	for (size_t i = 0; i < size; i++)
	{
		result->data[i] = (double*)calloc(size, sizeof(double));

		if (!result->data[i])
		{
			printf("Can't allocate memory for matrix");
			return NULL;
		}
	}

	return result;
}

void free_matrixes(matrix* matrixes, size_t number)
{
	for (size_t i = 0; i < number; i++)
	{
		if (matrixes[i].data)
		{
			free_matrix(matrixes + i);
		}
		else
		{
			break;
		}
	}

	free(matrixes);
}

void print_matrix(FILE *file, matrix *matrix)
{
	for (size_t i = 0; i < matrix->size; i++)
	{
		for (size_t j = 0; j < matrix->size; j++)
		{
			fprintf(file, "%3.3lf ", matrix->data[i][j]);
		}
		fprintf(file, "\n");
	}
}

void print_matrixes(FILE *file, matrix *matrixes, size_t number)
{
	for (size_t i = 0; i < number; i++)
	{
		fprintf(file, "%d\n", (matrixes + i)->size);
		print_matrix(file, matrixes + i);
	}
}

void free_matrix(matrix* M)
{
	if (M->data)
	{
		for (size_t j = 0; j < M->size; j++)
		{
			if (M->data[j])
			{
				free(M->data[j]);
			}
			else
			{
				break;
			}
		}
		free(M->data);
	}
}
