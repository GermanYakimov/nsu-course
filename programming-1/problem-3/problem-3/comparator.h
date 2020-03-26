#pragma once
#include<time.h>

double *run_sort(char sort, int calls_number, void *base, size_t num, size_t size, int(*compar)(void*, void*), void*(*prepare_data)(void*, int));
double standard_dev_count(double *data, int size);
