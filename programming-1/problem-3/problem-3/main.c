#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

typedef struct matrix {
	int size;
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
	matrix result;
	result.size = size;

	result.data = (int**)calloc(size, sizeof(int*));
	if (!result.data)
	{
		printf("Can't allocate memory for matrix");
		return NULL;
	}

	for (int i = 0; i < size; i++)
	{
		result.data[i] = (int*)calloc(size, sizeof(int));

		if (!result.data[i])
		{
			printf("Can't allocate memory for matrix");
			return NULL;
		}
	}

	return &result;
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
		print_matrix(matrixes[i]);
	}

	fclose(input);

	free_matrixes(matrixes, size);
	return 0;
}