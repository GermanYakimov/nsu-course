#include<stdlib.h>
#include <time.h>
#include "matrix.h"

matrix *generate_matrix(short max_size, int upper_limit)
{
	int size = 1 + rand() % max_size;

	matrix *result = allocate_matrix(size);

	if (!result)
	{
		return NULL;
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			result->data[i][j] = rand() % upper_limit;
		}
	}

	return result;
}

matrix *generate_matrix_array(short max_size, int upper_limit, int number)
{
	matrix *result = (matrix*)malloc(number * sizeof(matrix));
	matrix *tmp;

	for (int i = 0; i < number; i++)
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

