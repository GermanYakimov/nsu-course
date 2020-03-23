#pragma once
#include<stdlib.h>

void swap(void *first, void *second, size_t size);
void *bubble_sort(void *base, size_t num, size_t size, int(*compar)(void*, void*));
