#include <stdlib.h>

#include "sorts.h"
#include "swap.h"

void *_make_heap(void *base, size_t num, size_t size, double(*compare)(void*, void*))
{
	int i, index, parent;
	char *arr = (char*)base;

	for (i = 0; i < (int)num; i++)
	{
		index = i;

		while (index != 0)
		{
			parent = (index - 1) / 2;

			if (0 <= parent <= (int)num && compare(arr + index * size, arr + parent * size) <= 0)
			{
				break;
			}
			else
			{
				swap(arr + index * size, arr + parent * size, size);
				index = parent;
			}
		}
	}

	return arr;
}

void *_fix_heap(void *base, size_t num, size_t size, double(*compare)(void*, void*))
{
	int index = 0, child_1, child_2;
	_Bool cond_1, cond_2;
	char *arr = (char*)base;

	while (1)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		cond_1 = child_1 >= (int)num;
		cond_2 = child_2 >= (int)num;

		if (cond_1 && cond_2)
		{
			return arr;
		}
		else if (cond_2)
		{
			if (compare(arr + child_1 * size, arr + index * size) > 0)
			{
				swap(arr + child_1*size, arr + index*size, size);
				index = child_1;
			}
			else
			{
				return arr;
			}
		}
		else
		{
			if (compare(arr + child_2*size, arr + child_1*size) >= 0)
			{
				if (compare(arr + child_2*size, arr + index*size) > 0)
				{
					swap(arr + child_2*size, arr + index*size, size);
					index = child_2;
				}
				else
				{
					return arr;
				}
			}
			else
			{
				if (compare(arr + child_1 * size, arr + index * size) > 0)
				{
					swap(arr + child_1*size, arr + index*size, size);
					index = child_1;
				}
				else
				{
					return arr;
				}
			}
		}
	}
}

void *heap_sort(void *data)
{
	sort_input *input = (sort_input*)data;

	int i, heap_start = 0, border = 1;
	char *arr = (char*)input->base;

	arr = (char*)_make_heap(input->base, input->num, input->size, input->compare);

	for (i = 0; i < (int)(input->num - 1); i++)
	{
		swap(arr, arr + (input->num - border) * input->size, input->size);
		arr = (char*)_fix_heap(arr, input->num - border, input->size, input->compare);
		border++;
	}

	return data;
}