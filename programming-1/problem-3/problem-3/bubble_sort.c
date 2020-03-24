#include "swap.h"
#include "sorts.h"

void *bubble_sort(void *base, size_t num, size_t size, int(*compar)(void*, void*))
{
	char *arr = (char*)base;
	int swaps_counter = 1;

	while (swaps_counter)
	{
		swaps_counter = 0;

		for (int i = 0; i < (int)(num - 1); i++)
		{
			if (compar(arr + i*size, arr + (i + 1)*size) > 0)
			{
				swap(arr + i*size, arr + (i + 1)*size, size);
				swaps_counter++;
			}
		}
	}

	return arr;
}
