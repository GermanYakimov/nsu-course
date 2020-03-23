#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

typedef struct matrix {
	short size;
	long det;
	int **data;
} matrix;

void swap(void *first, void *second, size_t size)
{
	size_t _size = size;
	char *_first = (char*)first, *_second = (char*)second, _tmp;
	do
	{
		_tmp = *_first;
		*_first++ = *_second;
		*_second++ = _tmp;
	} while (--_size > 0);
}

int greater(void* one, void* two)
{
	matrix* one_tmp = (matrix*)one;
	matrix* two_tmp = (matrix*)two;
	return one_tmp->det - two_tmp->det;
}

void print_matrix(FILE *file, matrix *matrix)
{
	for (int i = 0; i < matrix->size; i++)
	{
		for (int j = 0; j < matrix->size; j++)
		{
			fprintf(file, "%d ", matrix->data[i][j]);
		}
		fprintf(file, "\n");
	}
}

void *make_heap(void *base, size_t num, size_t size, int(*compar)(void*, void*))
{
	int i, index, parent;
	char *arr = (char*)base;

	for (i = 0; i < num; i++)
	{
		index = i;

		while (index != 0)
		{
			parent = (index - 1) / 2;

			if (0 <= parent <= num && compar(arr + index * size, arr + parent * size) <= 0)
			{
				break;
			}
			else
			{
				swap(arr + index * size, arr + parent * size, size);
				index = parent;
			}
		}
	}

	return arr;
}

void *fix_heap(void *base, size_t num, size_t size, int(*compar)(void*, void*))
{
	int index = 0, child_1, child_2;
	int cond_1, cond_2;
	char *arr = (char*)base;

	while (1)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		cond_1 = child_1 >= num;
		cond_2 = child_2 >= num;

		if (cond_1 && cond_2)
		{
			break;
		}
		else if (cond_2)
		{
			if (compar(arr + child_1 * size, arr + index * size) > 0)
			{
				swap(arr + child_1*size, arr + index*size, size);
				index = child_1;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (compar(arr + child_2*size, arr + child_1*size) >= 0)
			{
				if (compar(arr + child_2*size, arr + index*size) > 0)
				{
					swap(arr + child_2*size, arr + index*size, size);
					index = child_2;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (compar(arr + child_1 * size, arr + index * size) > 0)
				{
					swap(arr + child_1*size, arr + index*size, size);
					index = child_1;
				}
				else
				{
					break;
				}
			}
		}
	}

	return arr;
}

void *sort(void *base, size_t num, size_t size, int(*compar)(void*, void*))
{
	int i, heap_start = 0, border = 1;
	char *arr = (char*)base;

	arr = (char*)make_heap(base, num, size, greater);

	for (i = 0; i < num - 1; i++)
	{
		swap(arr, arr + (num - border) * size, size);
		arr = (char*)fix_heap(arr, num - border, size, greater);
		border++;
	}

	return arr;
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

matrix* allocate_matrix(int size)
{
	matrix *result = (matrix*)malloc(sizeof(matrix));
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

void print_matrix_1(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
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

	return result;
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

int main()
{
	int size;
	matrix *matrixes;
	FILE *input, *output;
	input = fopen("input.txt", "r");

	if (!input)
	{
		printf("Can't open input.txt");
		return -1;
	}

	fscanf(input, "%d", &size);
	matrixes = read_matrixes(input, size);
	fclose(input);

	if (!matrixes)
	{
		return -1;
	}

	for (int i = 0; i < size; i++)
	{
		matrixes[i].det = det(matrixes + i);

		if (matrixes[i].det == LONG_MAX)
		{
			return -1;
		}
	}

	matrixes = (matrix*)sort(matrixes, size, sizeof(matrix), greater);

	output = fopen("output.txt", "w");

	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	for (int i = 0; i < size; i++)
	{
		print_matrix(output, &matrixes[i]);
	}

	fclose(output);

	free_matrixes(matrixes, size);
	return 0;
}