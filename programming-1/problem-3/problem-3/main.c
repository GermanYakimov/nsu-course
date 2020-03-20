#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

typedef struct matrix {
	int size;
	int det;
	int **data;
} matrix;

void swap(int *first, int *second)
{
	int tmp = *first;
	*first = *second;
	*second = tmp;
}

int *make_heap(int *arr, int size)
{
	int i, index, parent;

	for (i = 0; i < size; i++)
	{
		index = i;

		while (index != 0)
		{
			parent = (index - 1) / 2;

			if (0 <= parent <= size && arr[index] <= arr[parent])
			{
				break;
			}
			else
			{
				swap(arr + index, arr + parent);
				index = parent;
			}
		}
	}

	return arr;
}

int *fix_heap(int* arr, int size)
{
	int index = 0, child_1, child_2;
	int cond_1, cond_2;

	while (1)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		cond_1 = child_1 >= size;
		cond_2 = child_2 >= size;

		if (cond_1 && cond_2)
		{
			break;
		}
		else if (cond_2)
		{
			if (arr[child_1] > arr[index])
			{
				swap(arr + child_1, arr + index);
				index = child_1;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (arr[child_2] >= arr[child_1])
			{
				if (arr[child_2] > arr[index])
				{
					swap(arr + child_2, arr + index);
					index = child_2;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (arr[child_1] > arr[index])
				{
					swap(arr + child_1, arr + index);
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

int *sort(int *numbers, int size)
{
	int i, heap_start = 0, border = 1;

	numbers = make_heap(numbers, size);

	for (i = 0; i < size - 1; i++)
	{
		swap(numbers, numbers + size - border);
		numbers = fix_heap(numbers, size - border);
		border++;
	}

	return numbers;
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

void print_matrix(matrix matrix)
{
	for (int i = 0; i < matrix.size; i++)
	{
		for (int j = 0; j < matrix.size; j++)
		{
			printf("%d ", matrix.data[i][j]);
		}
		printf("\n");
	}
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
		result = result +  (-1)*(-1 + 2 * (i % 2)) * M[0][i] * det(delete_row_and_column(M, tmp_matrix, 0, i, size), size - 1);
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

int greater(matrix one, matrix two)
{
	return one.det > two.det;
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

	fclose(input);

	free_matrixes(matrixes, size);
	return 0;
}