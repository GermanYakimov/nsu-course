#include "swap.h"
#include "sorts.h"

void *bubble_sort(void *data)
{
	sort_input *input = (sort_input*)data;

	char *arr = (char*)input->base;
	int swaps_counter = 1;

	while (swaps_counter)
	{
		swaps_counter = 0;

		for (int i = 0; i < (int)(input->num - 1); i++)
		{
			if (input->compare(arr + i*input->size, arr + (i + 1)*input->size) > 0)
			{
				swap(arr + i*input->size, arr + (i + 1)*input->size, input->size);
				swaps_counter++;
			}
		}
	}

	return data;
}
