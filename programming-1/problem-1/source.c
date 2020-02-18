#include<stdio.h>
#include<stdlib.h>

int is_max_in_column(int index, int column_number, int** matrix, int str_count)
{
	for (int i = 0; i < str_count; i++)
	{
		if (matrix[i][column_number] > matrix[index][column_number])
		{
			return 0;
		}
	}

	return 1;
}

int find_max_in_column(int** matrix, int column, int strings_count)
{
	int current_max = matrix[0][column];

	for (int i = 1; i < strings_count; i++)
	{
		if (matrix[i][column] > current_max)
		{
			current_max = matrix[i][column];
		}
	}

	return current_max;
}

int find_min_in_str(int* str, int size)
{
	int current_min = str[0];

	for (int i = 1; i < size; i++)
	{
		if (str[i] < current_min)
		{
			current_min = str[i];
		}
	}

	return current_min;
}

int average_in_column(int** matrix, int column, int str_count)
{
	int sum = 0;
	for (int i = 0; i < str_count; i++)
	{
		sum += matrix[i][column];
	}

	return sum / str_count;
}

int count_points(int strings_count, int columns_count, int** matrix)
{
	int min_in_str;
	int points_count = 0;

	int* min_elements_in_strings = (int*)malloc(strings_count * sizeof(int));
	int* max_elements_in_columns = (int*)malloc(columns_count * sizeof(int));

	if (!min_elements_in_strings || !max_elements_in_columns)
	{
		printf("Can't allocate memory for arrays in count_points\n");
		return -1;
	}

	for (int i = 0; i < strings_count; i++)
		min_elements_in_strings[i] = find_min_in_str(matrix[i], columns_count);

	for(int i = 0; i < columns_count; i++)
		max_elements_in_columns[i] = find_max_in_column(matrix, i, strings_count);

	for (int i = 0; i < strings_count; i++)
	{
		for(int j = 0; j < columns_count; j++)
		{
			if (matrix[i][j] == min_elements_in_strings[i] && matrix[i][j] == max_elements_in_columns[j])
			{
				matrix[i][j] = average_in_column(matrix, j, strings_count);
				points_count++;
			}
		}
	}

	free(min_elements_in_strings);
	free(max_elements_in_columns);

	return points_count;
}

int** allocate_matrix(int strings_count, int columns_count)
{
	int** matrix = (int**)malloc(strings_count * sizeof(int*));

	if (!matrix)
	{
		printf("Can't allocate memory for matrix");
		return NULL;
	}

	for (int i = 0; i < strings_count; i++)
	{
		matrix[i] = (int*)malloc(columns_count * sizeof(int));

		if (!matrix[i])
		{
			printf("Can't allocate memory for matrix");
			return NULL;
		}
	}

	return matrix;
}

void free_matrix(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
		free(matrix[i]);

	free(matrix);
}

void print_matrix(FILE* file, int** matrix, int strings_count, int columns_count)
{
	for (int i = 0; i < strings_count; i++)
	{
		for(int j = 0; j < columns_count; j++)
		{
			fprintf(file, "%d ", matrix[i][j]);
		}
		fprintf(file, "\n");
	}
}


int main()
{
	short columns_count, strings_count;
	int points_count = 0;

	FILE *input, *output;
	input = fopen("input.txt", "r");

	if (!input)
	{
		printf("Can't open input file\n");
		return 1;
	}

	fscanf(input, "%hi %hi", &columns_count, &strings_count);

	int** matrix = allocate_matrix(strings_count, columns_count);

	if (!matrix)
		return 1;

	for (int i = 0; i < strings_count; i++)
		for (int j = 0; j < columns_count; j++)
		{
			fscanf(input, "%d", &matrix[i][j]);
		}

	fclose(input);

	output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Can't open output file\n");
		return 1;
	}

	points_count = count_points(strings_count, columns_count, matrix);
	if (points_count == -1)
	{
		return -1;
	}

	fprintf(output, "%d\n", points_count);
	print_matrix(output, matrix, strings_count, columns_count);
	fclose(output);

	free_matrix(matrix, strings_count);

	return 0;
}
