#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matrix.h"
#include "swap.h"
#include "comparator.h"
#include "input_generator.h"
#include "sorts.h"

double greater_det(void* one, void* two)
{
	matrix* one_tmp = (matrix*)one;
	matrix* two_tmp = (matrix*)two;
	return one_tmp->det - two_tmp->det;
}

matrix *read_matrixes(FILE *input, size_t size, size_t max_order)
{
	matrix *matrixes = (matrix*)calloc(size, sizeof(matrix));

	if (!matrixes)
	{
		printf("Can't allocate memory for matrixes");
		return NULL;
	}

	matrix *tmp;

	size_t order;

	for (size_t i = 0; i < size; i++)
	{
		fscanf(input, "%lu", &order);
		tmp = allocate_matrix(max_order);
		tmp->size = order;

		if (!tmp)
		{
			free_matrixes(matrixes, size);
			return NULL;
		}

		matrixes[i] = *tmp;
		free(tmp);

		for (size_t j = 0; j < order; j++)
		{
			for (size_t k = 0; k < order; k++)
			{
				fscanf(input, "%lf", (&matrixes[i].data[j][k]));
			}
		}
	}

	return matrixes;
}

void *load_matrixes(FILE *input, sort_input *data)
{
	size_t size, order;
	fscanf(input, "%lu %lu", &size, &order);
	matrix *matrixes = (matrix*)data->base;

	for (size_t i = 0; i < size; i++)
	{
		fscanf(input, "%lu", &order);

		for (size_t j = 0; j < order; j++)
		{
			for (size_t k = 0; k < order; k++)
			{
				fscanf(input, "%lf", (&matrixes[i].data[j][k]));
			}
		}

		fscanf(input, "%lf", &matrixes[i].det);
	}

	return data;
}

void run_matrixes_sort_benchmark()
{
	benchmark_res result;
	matrix *matrixes;

	sort_input input;
	input.compare = greater_det;
	input.size = sizeof(matrix);

	int upper_limit;
	size_t calls_number, size, max_order;
	char filename[256];
	char generate_random_input, sort;

	printf("select the algorithm: ");
	getchar();
	scanf("%c", &sort);
	printf("generate random input (y/n)? ");
	getchar();
	scanf("%c", &generate_random_input);

	if (generate_random_input == 'y')
	{
		printf("matrix number: ");
		scanf("%lu", &size);
		printf("upper limit: ");
		scanf("%d", &upper_limit);
		printf("max matrix size: ");
		scanf("%lu", &max_order);

		matrixes = generate_matrix_array(max_order, upper_limit, size);
		FILE *input_backup = fopen("last_generated_input.txt", "w");

		if (!input_backup)
		{
			printf("Can't backup generated input");
			return;
		}

		print_matrixes(input_backup, matrixes, size, 0, max_order);
		fclose(input_backup);

		if (!matrixes)
		{
			return;
		}
	}
	else if (generate_random_input == 'n')
	{
		printf("filename (to enter data manually type m, to use last generated input type l): ");
		scanf("%s", filename);

		if (strlen(filename) == 1)
		{
			if (filename[0] == 'm')
			{
				printf("matrix number: ");
				scanf("%lu", &size);
				printf("max matrix order: ");
				scanf("%lu", &max_order);
				printf("then enter the matrixes:\n");
				matrixes = read_matrixes(stdin, size, max_order);

				if (!matrixes)
				{
					return;
				}
			}
			else if (filename[0] == 'l')
			{
				FILE *last_input = fopen("last_generated_input.txt", "r");

				if (!last_input)
				{
					printf("Can't find file with input (last_generated_input.txt)");
					return;
				}

				fscanf(last_input, "%lu %lu", &size, &max_order);

				matrixes = read_matrixes(last_input, size, max_order);

				if (!matrixes)
				{
					fclose(last_input);
					return;
				}

				fclose(last_input);
			}
		}
		else
		{
			FILE *input = fopen(filename, "r");

			if (!input)
			{
				printf("Can't open %s", filename);
				return;
			}

			fscanf(input, "%lu %lu", &size, &max_order);

			matrixes = read_matrixes(input, size, max_order);
			fclose(input);
		}
	}

	printf("calls number: ");
	scanf("%lu", &calls_number);

	count_matrixes_dets(matrixes, size);

	input.num = size;
	input.base = matrixes;

	char backup_filename[64] = "backup.txt";
	FILE *backup = fopen("backup.txt", "w");
	if (!backup)
	{
		printf("Can't open file for matrixes backup");
		return;
	}

	print_matrixes(backup, matrixes, size, 1, max_order);
	fclose(backup);

	switch (sort)
	{
	case 'b':
		result = benchmark(calls_number, &input, bubble_sort, load_matrixes, backup_filename);
		break;
	case 'h':
		result = benchmark(calls_number, &input, heap_sort, load_matrixes, backup_filename);
		break;
	case 'q':
		result = benchmark(calls_number, &input, quick_sort, load_matrixes, backup_filename);
		break;
	default:
		break;
	}
	print_benchmark_result(stdout, result);

	//free_matrixes(matrixes, size);
	free_benchmark_result(result);
}

matrix *copy_matrix(matrix *M)
{
	matrix *result = allocate_matrix(M->size);

	if (!result)
	{
		printf("Can't copy matrix");
		return NULL;
	}
	
	for (size_t i = 0; i < M->size; i++)
	{
		for (size_t j = 0; j < M->size; j++)
		{
			result->data[i][j] = M->data[i][j];
		}
	}

	return result;
}

matrix *count_matrixes_dets(matrix *matrixes, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		matrixes[i].det = det_quick(matrixes + i);
	}

	return matrixes;
}

double det(matrix *M)
{
	
	if (M->size == 1)
	{
		return M->data[0][0];
	}

	if (M->size == 2)
	{
		return M->data[0][0] * M->data[1][1] - M->data[1][0] * M->data[0][1];
	}

	double result = 0;

	matrix *tmp_matrix = allocate_matrix(M->size - 1);

	if (!tmp_matrix)
	{
		printf("Can't allocate memory for additional matrix");
		return LONG_MAX;
	}

	for (size_t i = 0; i < M->size; i++)
	{
		result = result + (-1)*(-1 + 2 * (i % 2)) * M->data[0][i] * det(additional_matrix(M, tmp_matrix, 0, i));
	}

	free_matrix(tmp_matrix);
	free(tmp_matrix);

	return result;
}

double det_quick(matrix *M)
{
	if (M->size == 1)
	{
		return M->data[0][0];
	}

	if (M->size == 2)
	{
		return M->data[0][0] * M->data[1][1] - M->data[1][0] * M->data[0][1];
	}

	size_t j;
	int max_index;
	double result = 1;

	matrix* M_copy = copy_matrix(M);

	for (size_t i = 0; i < M_copy->size; i++)
	{
		j = i;
		max_index = i;

		for (size_t k = i; k < M_copy->size; k++)
		{
			if (fabs(M_copy->data[k][i]) > fabs(M_copy->data[max_index][i]))
			{
				max_index = k;
			}
		}
		if (M_copy->data[max_index][i] == 0)
		{
			return 0;
		}

		if (max_index != i)
		{
			swap(M_copy->data + i, M_copy->data + max_index, sizeof(M_copy->data[max_index]));
			result *= -1;
		}

		for (size_t k = i + 1; k < M_copy->size; k++)
		{
			M_copy->data[i][k] /= M_copy->data[i][i];
		}
		result *= M_copy->data[i][i];

		M_copy->data[i][i] = 1;

		for (size_t k = i + 1; k < M_copy->size; k++)
		{
			for (size_t s = i + 1; s < M_copy->size; s++)
			{
				M_copy->data[k][s] += M_copy->data[i][s] * (-M_copy->data[k][i]);
			}
			M_copy->data[k][i] += M_copy->data[i][i] * (-M_copy->data[k][i]);
		}
	}

	free_matrix(M_copy);

	return result;
}

matrix *additional_matrix(matrix *M, matrix *result, int row, int column)
{
	int tmp_i = 0, tmp_j = 0;

	for (size_t i = 0; i < M->size; i++)
	{
		if (i == row)
		{
			continue;
		}
		tmp_j = 0;

		for (size_t j = 0; j < M->size; j++)
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

matrix* allocate_matrix(size_t size)
{
	matrix *result = (matrix*)malloc(sizeof(matrix));
	if (!result)
	{
		printf("403: Can't alocate memory for matrix");
		return NULL;
	}

	result->size = size;

	result->data = (double**)calloc(size, sizeof(double*));
	if (!result->data)
	{
		printf("412: Can't allocate memory for matrix");
		return NULL;
	}

	for (size_t i = 0; i < size; i++)
	{
		result->data[i] = (double*)calloc(size, sizeof(double));

		if (!result->data[i])
		{
			printf("422: Can't allocate memory for matrix");
			return NULL;
		}
	}

	return result;
}

void free_matrixes(matrix* matrixes, size_t number)
{
	for (size_t i = 0; i < number; i++)
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

void print_matrix(FILE *file, matrix *matrix, _Bool with_det)
{
	for (size_t i = 0; i < matrix->size; i++)
	{
		for (size_t j = 0; j < matrix->size; j++)
		{
			fprintf(file, "%lf ", matrix->data[i][j]);
		}
		fprintf(file, "\n");
	}

	if(with_det)
	{
		fprintf(file, "%lf\n", matrix->det);
	}
}

void print_matrixes(FILE *file, matrix *matrixes, size_t number, _Bool with_dets, size_t max_order)
{
	fprintf(file, "%lu %lu\n", number, max_order);
	for (size_t i = 0; i < number; i++)
	{
		fprintf(file, "%d\n", (matrixes + i)->size);
		print_matrix(file, matrixes + i, with_dets);
	}
}

void free_matrix(matrix* M)
{
	if (M->data)
	{
		for (size_t j = 0; j < M->size; j++)
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
