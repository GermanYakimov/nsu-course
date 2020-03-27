#pragma once

#include <time.h>
#include <stdio.h>

typedef struct benchmark_result {
	char algorithm;
	int data_size;
	int calls_number;
	int best_run;
	int worst_run;
	double *runtime;
	double average_time;
	double standard_dev_1;
	double standard_dev_2;
} benchmark_res;

double count_average(double *runtime, int num);
double *run_sort(char sort, int calls_number, void *base, size_t num, size_t size, int(*compar)(void*, void*), void*(*prepare_data)(void*, int));
double count_standard_dev_1(double *runtime, double average, int num);
double count_standard_dev_2(double *runtime, double average, int num);
benchmark_res benchmark(char sort, int calls_number, void *base, size_t num, size_t size, int(*compar)(void*, void*), void*(*prepare_data)(void*, int));
void free_benchmark_result(benchmark_res result);
void print_benchmark_result(FILE *file, benchmark_res result);
int find_best_run(double *runtime, int num);
int find_worst_run(double *runtime, int num);
