#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorts.h"
#include "matrix.h"
#include "comparator.h"
#include "input_generator.h"

void run_benchmark()
{
	benchmark_res result;
	matrix *matrixes;

	short max_matrix_size;
	int size, upper_limit, calls_num;
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
		printf("matrix count: ");
		scanf("%d", &size);
		printf("upper limit: ");
		scanf("%d", &upper_limit);
		printf("max matrix size: ");
		scanf("%hi", &max_matrix_size);

		matrixes = generate_matrix_array(max_matrix_size, upper_limit, size);

		if (!matrixes)
		{
			return;
		}
	}
	else if (generate_random_input == 'n')
	{
		printf("enter filename (if you want to enter data manually, type 'm'): ");
		scanf("%s", filename);

		if (strlen(filename) == 1 && filename[0] == 'm')
		{
			scanf("%d", &size);
			matrixes = read_matrixes(stdin, size);

			if (!matrixes)
			{
				return;
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

			fscanf(input, "%d", &size);
			matrixes = read_matrixes(input, size);
		}
	}

	printf("calls number: ");
	scanf("%d", &calls_num);

	result = benchmark(sort, calls_num, matrixes, size, sizeof(matrix), greater_det, count_matrixes_dets);
	print_benchmark_result(stdout, result);

	free_matrixes(matrixes, size);
	free_benchmark_result(result);
}

int main()
{
	srand(time(0));
	char option;

	printf("select an option(b - benchmark test): ");
	scanf("%c", &option);

	switch (option)
	{
	case 'b':
		run_benchmark();
		break;
	}

	return 0;
}