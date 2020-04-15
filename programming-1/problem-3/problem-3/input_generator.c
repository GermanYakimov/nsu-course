#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "matrix.h"

matrix *generate_matrix(size_t max_size, int upper_limit)
{
	matrix *result = allocate_matrix(max_size);
	result->size = 1 + rand() % max_size;

	if (!result)
	{
		return NULL;
	}

	for (size_t i = 0; i < result->size; i++)
	{
		for (size_t j = 0; j < result->size; j++)
		{
			result->data[i][j] = (int)(pow(-1.0, (rand() % 2) / 1.0)) * rand() % upper_limit;
		}
	}

	return result;
}

matrix *generate_matrix_array(size_t max_size, int upper_limit, size_t num)
{
	matrix *result = (matrix*)malloc(num * sizeof(matrix));
	matrix *tmp;

	for (size_t i = 0; i < num; i++)
	{
		tmp = generate_matrix(max_size, upper_limit);

		if (!tmp)
		{
			return NULL;
		}

		result[i] = *tmp;
		free(tmp);
	}

	return result;
}

int *generate_numbers_array(int upper_limit, size_t num)
{
	int *result = (int*)malloc(num * sizeof(int));

	for (size_t i = 0; i < num; i++)
	{
		result[i] = (int)(pow(-1.0, (rand() % 2) / 1.0)) * rand() % upper_limit;
	}

	return result;
}
