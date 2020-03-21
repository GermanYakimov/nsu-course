#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

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

typedef struct matrix {
	short size;
	short det;
	int **data;
} matrix;


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
				//print_matrix(arr + index * size);
				//print_matrix(arr + parent * size);
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

	//for (int i = 0; i < num; i++)
	//{
	//	print_matrix(arr + i*size);
	//}

	for (i = 0; i < num - 1; i++)
	{
		swap(arr, arr + (num - border) * size, size);
		//print_matrix(arr);
		//print_matrix(arr + (num - border) * size);
		arr = (char*)fix_heap(arr, num - border, size, greater);
		border++;
	}

	return arr;
}

void free_matrixes(matrix* matrixes, int number)
{
	for (int i = 0; i < number; i++)
	{
		if (matrixes[i].data)
		{
			for (int j = 0; j < matrixes[i].size; j++)
			{
				if (matrixes[i].data[j])
				{
					free(matrixes[i].data[j]);
				}
				else
				{
					break;
				}
			}
			free(matrixes[i].data);
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

int **delete_row_and_column(int **M, int **result, int row, int column, int size)
{
	int tmp_i = 0, tmp_j = 0;

	for (int i = 0; i < size; i++)
	{
		if (i == row)
		{
			continue;
		}
		tmp_j = 0;

		for (int j = 0; j < size; j++)
		{
			if (j == column)
			{
				continue;
			}
			result[tmp_i][tmp_j] = M[i][j];
			tmp_j++;
		}

		tmp_i++;

	}

	//print_matrix_1(result, size - 1);
	return result;
}

int det(int **M, int size)
{
	if (size == 1)
	{
		return M[0][0];
	}

	if (size == 2)
	{
		return M[0][0] * M[1][1] - M[1][0] * M[0][1];
	}

	int result = 0;
	int **tmp_matrix = (int**)calloc(size - 1, sizeof(int*));

	for (int i = 0; i < size - 1; i++)
	{
		tmp_matrix[i] = (int*)malloc((size - 1) * sizeof(int));
	}

	for (int i = 0; i < size; i++)
	{
		result = result + (-1)*(-1 + 2 * (i % 2)) * M[0][i] * det(delete_row_and_column(M, tmp_matrix, 0, i, size), size - 1);
	}

	for (int i = 0; i < size - 1; i++)
	{
		free(tmp_matrix[i]);
	}

	free(tmp_matrix);

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
		//print_matrix(matrixes[i]);
		matrixes[i].det = det(matrixes[i].data, matrixes[i].size);
		//printf("det: %d\n", det(matrixes[i].data, matrixes[i].size));
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