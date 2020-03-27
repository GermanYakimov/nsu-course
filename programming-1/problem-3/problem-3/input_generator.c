#include<stdlib.h>
#include <time.h>
#include "matrix.h"

matrix *generate_matrix(short max_size, int upper_limit)
{
	short size = 1 + rand() % max_size;

	matrix *result = allocate_matrix(size);

	if (!result)
	{
		return NULL;
	}

	for (short i = 0; i < size; i++)
	{
		for (short j = 0; j < size; j++)
		{
			result->data[i][j] = rand() % upper_limit;
		}
	}

	return result;
}

matrix *generate_matrix_array(short max_size, int upper_limit, int num)
{
	matrix *result = (matrix*)malloc(num * sizeof(matrix));
	matrix *tmp;

	for (int i = 0; i < num; i++)
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

