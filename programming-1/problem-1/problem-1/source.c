#include<stdio.h>
#include<stdlib.h>

int find_max_in_column(int** matrix, int column, int rows_count)
{
	int current_max = matrix[0][column];

	for (int i = 1; i < rows_count; i++)
	{
		if (matrix[i][column] > current_max)
		{
			current_max = matrix[i][column];
		}
	}

	return current_max;
}

int find_min_in_row(int** matrix, int row, int columns_count)
{
	int current_min = matrix[row][0];

	for (int i = 1; i < columns_count; i++)
	{
		if (matrix[row][i] < current_min)
		{
			current_min = matrix[row][i];
		}
	}

	return current_min;
}

int average_in_column(int** matrix, int column, int rows_count)
{
	int sum = 0;

	for (int i = 0; i < rows_count; i++)
	{
		sum += matrix[i][column];
	}

	return sum / rows_count;
}

int count_points(int rows_count, int columns_count, int** matrix)
{
	int min_in_str;
	int points_count = 0;

	int* min_elements_in_rows = (int*)calloc(rows_count * sizeof(int));
	int* max_elements_in_columns = (int*)calloc(columns_count * sizeof(int));

	if (!min_elements_in_rows || !max_elements_in_columns)
	{
		printf("Can't allocate memory for arrays in count_points\n");
		return -1;
	}

	for (int i = 0; i < rows_count; i++)
	{
		min_elements_in_rows[i] = find_min_in_row(matrix, i, columns_count);
	}

	for (int i = 0; i < columns_count; i++)
	{
		max_elements_in_columns[i] = find_max_in_column(matrix, i, rows_count);
	}

	for (int i = 0; i < rows_count; i++)
	{
		for (int j = 0; j < columns_count; j++)
		{
			if (matrix[i][j] == min_elements_in_rows[i] && matrix[i][j] == max_elements_in_columns[j])
			{
				matrix[i][j] = average_in_column(matrix, j, rows_count);
				points_count++;
			}
		}
	}

	free(min_elements_in_rows);
	free(max_elements_in_columns);

	return points_count;
}

void free_matrix(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (matrix[i])
		{
			free(matrix[i]);
		}
		else
		{
			break;
		}
	}

	free(matrix);
}

int** allocate_matrix(int rows_count, int columns_count)
{
	int** matrix = (int**)calloc(rows_count * sizeof(int*));

	if (!matrix)
	{
		printf("Can't allocate memory for matrix");
		return NULL;
	}

	for (int i = 0; i < rows_count; i++)
	{
		matrix[i] = (int*)calloc(columns_count * sizeof(int));

		if (!matrix[i])
		{
			printf("Can't allocate memory for matrix");
			free_matrix(matrix, rows_count);
			return NULL;
		}
	}

	return matrix;
}

void print_matrix(FILE* file, int** matrix, int rows_count, int columns_count)
{
	for (int i = 0; i < rows_count; i++)
	{
		for (int j = 0; j < columns_count; j++)
		{
			fprintf(file, "%d ", matrix[i][j]);
		}
		fprintf(file, "\n");
	}
}


int main()
{
	short columns_count, rows_count;
	int points_count = 0;

	FILE *input, *output;
	input = fopen("input.txt", "r");

	if (!input)
	{
		printf("Can't open input file\n");
		return 1;
	}

	fscanf(input, "%hi %hi", &columns_count, &rows_count);

	int** matrix = allocate_matrix(rows_count, columns_count);

	if (!matrix)
	{
		return 1;
	}

	for (int i = 0; i < rows_count; i++)
	{
		for (int j = 0; j < columns_count; j++)
		{
			fscanf(input, "%d", &matrix[i][j]);
		}
	}

	fclose(input);

	output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Can't open output file\n");
		return 1;
	}

	points_count = count_points(rows_count, columns_count, matrix);

	if (points_count == -1)
	{
		return -1;
	}

	fprintf(output, "%d\n", points_count);
	print_matrix(output, matrix, rows_count, columns_count);
	fclose(output);

	free_matrix(matrix, rows_count);

	return 0;
}
