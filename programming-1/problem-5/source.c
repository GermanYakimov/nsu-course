#include<stdio.h>
#include<stdlib.h>

int **allocate_matrix(int n)
{
	int **matrix = (int**)malloc(n * sizeof(int*));
	
	if (!matrix)
	{
		return NULL;
	}

	for (int i = 0; i < n; i++)
	{
		matrix[i] = (int*)malloc(n * sizeof(int));

		if (!matrix[i])
		{
			for (int j = 0; j < i; j++)
			{
				free(matrix[j]);
			}
			free(matrix);
			
			return NULL;
		}

		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = -1;
		}
	}

	return matrix;
}

void free_matrix(int **matrix, int size)
{
	for(int i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	
	int target, points, paths, from, to;
	FILE *input = fopen("input.txt", "r");
	
	if (!input)
	{
		printf("Can't open input.txt");
		return -1;
	} 

	fscanf(input, "%d %d %d", &target, &points, &paths);
	int **matrix = allocate_matrix(points);
	
	for(int i = 0; i < paths; i++)
	{
		fscanf(input, "%d %d", &from, &to);
		fscanf(input, "%d", &matrix[from][to]);
	}

	print_matrix(matrix, points);
	free_matrix(matrix, points);

	return 0;
}
