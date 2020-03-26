#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

int greater_det(void* one, void* two)
{
	matrix* one_tmp = (matrix*)one;
	matrix* two_tmp = (matrix*)two;
	return one_tmp->det - two_tmp->det;
}

matrix *read_matrixes(FILE *input, int size)
{
	matrix *matrixes = (matrix*)calloc(size, sizeof(matrix));

	if (!matrixes)
	{
		printf("Can't allocate memore for matrixes");
		return NULL;
	}

	matrix *tmp;

	int order;

	for (int i = 0; i < size; i++)
	{
		fscanf(input, "%d", &order);
		tmp = allocate_matrix(order);

		if (!tmp)
		{
			free_matrixes(matrixes, size);
			return NULL;
		}

		matrixes[i] = *tmp;
		free(tmp);

		for (int j = 0; j < order; j++)
		{
			for (int k = 0; k < order; k++)
			{
				fscanf(input, "%d", (&matrixes[i].data[j][k]));
			}
		}
	}

	return matrixes;
}

matrix *count_matrixes_dets(matrix *matrixes, int num)
{
	for (int i = 0; i < num; i++)
	{
		matrixes[i].det = det(matrixes + i);
	}

	return matrixes;
}

long det(matrix *M)
{
	if (M->size == 1)
	{
		return M->data[0][0];
	}

	if (M->size == 2)
	{
		return M->data[0][0] * M->data[1][1] - M->data[1][0] * M->data[0][1];
	}

	int result = 0;

	matrix *tmp_matrix = allocate_matrix(M->size - 1);

	if (!tmp_matrix)
	{
		printf("Can't allocate memory for additional matrix");
		return LONG_MAX;
	}

	for (int i = 0; i < M->size; i++)
	{
		result = result + (-1)*(-1 + 2 * (i % 2)) * M->data[0][i] * det(additional_matrix(M, tmp_matrix, 0, i));
	}

	free_matrix(tmp_matrix);
	free(tmp_matrix);

	return result;
}

matrix *additional_matrix(matrix *M, matrix *result, int row, int column)
{
	int tmp_i = 0, tmp_j = 0;

	for (int i = 0; i < M->size; i++)
	{
		if (i == row)
		{
			continue;
		}
		tmp_j = 0;

		for (int j = 0; j < M->size; j++)
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

matrix* allocate_matrix(int size)
{
	matrix *result = (matrix*)malloc(sizeof(matrix));
	if (!result)
	{
		printf("Can't alocate memory for matrix");
		return NULL;
	}

	result->size = size;

	result->data = (int**)calloc(size, sizeof(int*));
	if (!result->data)
	{
		printf("Can't allocate memory for matrix");
		return NULL;
	}

	for (int i = 0; i < size; i++)
	{
		result->data[i] = (int*)calloc(size, sizeof(int));

		if (!result->data[i])
		{
			printf("Can't allocate memory for matrix");
			return NULL;
		}
	}

	return result;
}

void free_matrixes(matrix* matrixes, int number)
{
	for (int i = 0; i < number; i++)
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
	fprintf(file, "\n");

	for (int i = 0; i < matrix->size; i++)
	{
		for (int j = 0; j < matrix->size; j++)
		{
			fprintf(file, "%d ", matrix->data[i][j]);
		}
		fprintf(file, "\n");
	}
}

void print_matrixes(FILE *file, matrix *matrixes, int number)
{
	for (int i = 0; i < number; i++)
	{
		print_matrix(file, matrixes + i);
	}
}

void free_matrix(matrix* M)
{
	if (M->data)
	{
		for (int j = 0; j < M->size; j++)
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
