#include <time.h>
#include <string.h>
#include <math.h>

#include "comparator.h"
#include "matrix.h"
#include "sorts.h"

double standard_dev_count(double *data, int size)
{
	double sum = 0, avg, result = 0;

	for (int i = 0; i < size; i++)
	{
		sum += data[i];
	}
	avg = sum / size;
	printf("average time: %lf\n", avg);
	
	for (int i = 0; i < size; i++)
	{
		result += (data[i] - avg) * (data[i] - avg);
	}

	result /= size;
	result = sqrt(result);

	return result;
}

double *run_sort(char sort, int calls_number, void *base, size_t num, size_t size, int(*compar)(void*, void*), void*(*prepare_data)(void*, int))
{
	void *base_copy = malloc(num*size);
	double *results = (double*)malloc(calls_number * sizeof(double));
	base_copy = memcpy(base_copy, base, num*size);
	clock_t time;

	switch (sort)
	{
	case 'h':
		for (int i = 0; i < calls_number; i++)
		{
			time = clock();
			base_copy = prepare_data(base_copy, num);
			base_copy = heap_sort(base_copy, num, size, compar);
			time = clock() - time;

			results[i] = (double)time;
			printf("%d: %lf\n", i, (double)time);

			base_copy = memcpy(base_copy, base, num*size);
		}
	}

	double standard_dev = standard_dev_count(results, calls_number);
	printf("standard deviation: %lf\n", standard_dev);
	free(base_copy);

	return results;
}
