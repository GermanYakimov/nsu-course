#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorts.h"
#include "matrix.h"
#include "comparator.h"
#include "input_generator.h"

#pragma comment(linker, "/STACK:100000000000")

int main()
{
	srand(time(0));
	char option;

	printf("select an option(b - benchmark test): ");
	scanf("%c", &option);

	switch (option)
	{
	case 'b':
		run_matrixes_sort_benchmark();
		break;
	}

	return 0;
}