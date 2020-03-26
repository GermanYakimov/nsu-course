#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#include "sorts.h"
#include "matrix.h"
#include "comparator.h"
#include "input_generator.h"

int main()
{
	srand(time(0));

	int size, upper_limit, max_matrix_size, calls_num;
	double *results;
	printf("matrix count: ");
	scanf("%d", &size);
	printf("upper limit: ");
	scanf("%d", &upper_limit);
	printf("max matrix size: ");
	scanf("%d", &max_matrix_size);
	printf("calls count: ");
	scanf("%d", &calls_num);

	matrix *matrixes = generate_matrix_array(max_matrix_size, upper_limit, size);

	if (!matrixes)
	{
		return -1;
	}

	results = run_sort('h', calls_num, matrixes, size, sizeof(matrix), greater_det, count_matrixes_dets);

	//print_matrixes(stdout, matrixes, size);

	//for (int i = 0; i < calls_num; i++)
	//{
	//	printf("%lf\n", results[i]);
	//}
	free(results);

	//printf("\n");

	//for (int i = 0; i < size; i++)
	//{
	//	matrixes[i].det = det(matrixes + i);

	//	if (matrixes[i].det == LONG_MAX)
	//	{
	//		return -1;
	//	}
	//}

	//matrixes = (matrix*)heap_sort(matrixes, size, sizeof(matrix), greater_det);

	//print_matrixes(stdout, matrixes, size);
	/*int size;
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

	matrixes = (matrix*)heap_sort(matrixes, size, sizeof(matrix), greater_det);
	
	double time = sorting_time(matrixes, size, sizeof(matrix), greater_det, heap_sort);
	printf("%lf\n", time);

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

	fclose(output);*/

	free_matrixes(matrixes, size);
	return 0;
}