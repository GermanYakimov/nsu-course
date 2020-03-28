#include <time.h>
#include <string.h>
#include <math.h>

#include "comparator.h"
#include "matrix.h"
#include "sorts.h"

double count_average(double *runtime, size_t num)
{
	double sum = 0;

	for (size_t i = 0; i < num; i++)
	{
		sum += runtime[i];
	}

	return sum / num;
}

double count_standard_dev_1(double *runtime, double average, size_t num)
{
	double result = 0;

	for (size_t i = 0; i < num; i++)
	{
		result += (runtime[i] - average) * (runtime[i] - average);
	}

	return sqrt(result / num);
}

double count_standard_dev_2(double *runtime, double average, size_t num)
{
	double result = 0;

	for (size_t i = 0; i < num; i++)
	{
		result += (runtime[i] - average) * (runtime[i] - average);
	}

	return sqrt(result / (num - 1));
}

double *run_sort(char sort, size_t calls_number, void *base, size_t num, size_t size, double(*compar)(void*, void*), void*(*prepare_data)(void*, int))
{
	void *base_copy = malloc(num*size);

	if (!base_copy)
	{
		printf("Can't allocate memory for the copy of input.");
		return NULL;
	}

	double *results = (double*)malloc(calls_number * sizeof(double));

	if (!results)
	{
		printf("Can't allocate memory for runtime array");
		return NULL;
	}

	base_copy = memcpy(base_copy, base, num*size);
	clock_t time;

	switch (sort)
	{
	case 'h':
		for (size_t i = 0; i < calls_number; i++)
		{
			time = clock();
			base_copy = prepare_data(base_copy, num);
			base_copy = heap_sort(base_copy, num, size, compar);
			time = clock() - time;

			results[i] = (double)time;

			base_copy = memcpy(base_copy, base, num*size);

		}
		break;
	case 'b':
		for (size_t i = 0; i < calls_number; i++)
		{
			time = clock();
			base_copy = prepare_data(base_copy, num);
			base_copy = bubble_sort(base_copy, num, size, compar);
			time = clock() - time;

			results[i] = (double)time;

			base_copy = memcpy(base_copy, base, num*size);
		}
		break;
	case 'q':
		for (size_t i = 0; i < calls_number; i++)
		{
			time = clock();
			base_copy = prepare_data(base_copy, num);
			base_copy = quick_sort(base_copy, num, size, compar);
			time = clock() - time;

			results[i] = (double)time;

			base_copy = memcpy(base_copy, base, num*size);
		}
		break;
	}

	free(base_copy);

	return results;
}

size_t find_worst_run(double *runtime, size_t num)
{
	size_t max_index = 0;

	for (size_t i = 1; i < num; i++)
	{
		if (runtime[i] > runtime[max_index])
		{
			max_index = i;
		}
	}

	return max_index;
}

size_t find_best_run(double *runtime, size_t num)
{
	size_t min_index = 0;

	for (size_t i = 1; i < num; i++)
	{
		if (runtime[i] < runtime[min_index])
		{
			min_index = i;
		}
	}

	return min_index;
}

benchmark_res benchmark(char sort, size_t calls_number, void *base, size_t num, size_t size, double(*compar)(void*, void*), void*(*prepare_data)(void*, int))
{
	benchmark_res result;
	result.data_size = num;
	result.algorithm = sort;
	result.calls_number = calls_number;

	result.runtime = run_sort(sort, calls_number, base, num, size, compar, prepare_data);
	result.average_time = count_average(result.runtime, calls_number);
	result.standard_dev_1 = count_standard_dev_1(result.runtime, result.average_time, calls_number);
	result.standard_dev_2 = count_standard_dev_2(result.runtime, result.average_time, calls_number);

	result.best_run = find_best_run(result.runtime, calls_number);
	result.worst_run = find_worst_run(result.runtime, calls_number);

	return result;
}

void print_benchmark_result(FILE *file, benchmark_res result)
{
	fprintf(file, "algorithm: %c\n", result.algorithm);

	for (size_t i = 0; i < result.calls_number; i++)
	{
		fprintf(file, "run %d: %lf\n", i, result.runtime[i]);
	}
	fprintf(file, "\n");

	fprintf(file, "average time (in ms): %lf\n\n", result.average_time);
	fprintf(file, "run %d is the best with time %lf\n", result.best_run, result.runtime[result.best_run]);
	fprintf(file, "run %d is the worst with time %lf\n\n", result.worst_run, result.runtime[result.worst_run]);
	fprintf(file, "uncorrected standard deviation: %lf\n", result.standard_dev_1);
	fprintf(file, "corrected standard deviation: %lf\n\n", result.standard_dev_2);
}

void free_benchmark_result(benchmark_res result)
{
	free(result.runtime);
}
