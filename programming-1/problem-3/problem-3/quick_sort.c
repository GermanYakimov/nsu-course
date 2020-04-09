#include <time.h>

#include "sorts.h"
#include "swap.h"

void _q_sort(void *start, size_t left, size_t right, size_t size, double(*compare)(void*, void*))
{
	if (left >= right)
	{
		return;
	}

	int pivot = left + rand() % (right - left + 1);
	swap((char*)start + pivot * size, (char*)start + left*size, size);
	int i = left, j = left;

	while (j < right)
	{
		j++;

		if (compare((char*)start + j*size, (char*)start + left*size) < 0)
		{
			i++;

			if (i != j)
			{
				swap((char*)start + i*size, (char*)start + j*size, size);
			}
		}
	}

	swap((char*)start + i*size, (char*)start + left*size, size);

	if ((int)left < i - 1)
	{
		_q_sort(start, left, (size_t)(i - 1), size, compare);
	}

	if (i + 1 < (int)right)
	{
		_q_sort(start, (size_t)(i + 1), right, size, compare);
	}
}

void *quick_sort(void *data)
{
	sort_input *input = (sort_input*)data;
	_q_sort(input->base, 0, input->num - 1, input->size, input->compare);

	return data;
}
