#pragma once

#include <stdlib.h>

typedef struct input_data_for_sort {
	void *base;
	size_t num;
	size_t size;
	double(*compare)(void*, void*);
} sort_input;

void *bubble_sort(void *data);
void *heap_sort(void *data);
void *quick_sort(void *data);
