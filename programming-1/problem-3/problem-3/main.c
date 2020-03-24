#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include "sorts.h"
#include "matrix.h"

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
		matrixes[i].det = det(matrixes + i);

		if (matrixes[i].det == LONG_MAX)
		{
			return -1;
		}
	}

	matrixes = (matrix*)heap_sort(matrixes, size, sizeof(matrix), greater_det);

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