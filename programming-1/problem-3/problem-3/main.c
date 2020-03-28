#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorts.h"
#include "matrix.h"
#include "comparator.h"
#include "input_generator.h"

#pragma comment(linker, "/STACK:100000000000")

void run_benchmark()
{
	benchmark_res result;
	matrix *matrixes;

	short max_matrix_size;
	int upper_limit;
	size_t calls_number, size;
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
		scanf("%hi", &max_matrix_size);

		matrixes = generate_matrix_array(max_matrix_size, upper_limit, size);
		FILE *input_backup = fopen("last_generated_input.txt", "w");

		if (!input_backup)
		{
			printf("Can't backup generated input");
			return;
		}

		fprintf(input_backup, "%lu\n", size);
		print_matrixes(input_backup, matrixes, size);
		fclose(input_backup);

		if (!matrixes)
		{
			return;
		}
	}
	else if (generate_random_input == 'n')
	{
		printf("enter filename (to enter data manually type m, to use last generated input type l): ");
		scanf("%s", filename);

		if (strlen(filename) == 1)
		{
			if (filename[0] == 'm')
			{
				printf("enter matrix number: ");
				scanf("%lu", &size);
				printf("then enter the matrixes:\n");
				matrixes = read_matrixes(stdin, size);

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

				fscanf(last_input, "%lu", &size);

				matrixes = read_matrixes(last_input, size);

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

			fscanf(input, "%lu", &size);

			matrixes = read_matrixes(input, size);
			fclose(input);
		}
	}

	printf("calls number: ");
	scanf("%lu", &calls_number);

	result = benchmark(sort, calls_number, matrixes, size, sizeof(matrix), greater_det, count_matrixes_dets);
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