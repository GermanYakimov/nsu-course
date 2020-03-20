#include<stdio.h>
#include<stdlib.h>

void swap(int *first, int *second)
{
	int tmp = *first;
	*first = *second;
	*second = tmp;
}

int *make_heap(int *arr, int size)
{
	int i, index, parent;

	for (i = 0; i < size; i++)
	{
		index = i;

		while (index != 0)
		{
			parent = (index - 1) / 2;

			if (0 <= parent <= size && arr[index] <= arr[parent])
			{
				break;
			}
			else
			{
				swap(arr + index, arr + parent);
				index = parent;
			}
		}
	}

	return arr;
}

int *fix_heap(int* arr, int size)
{
	int index = 0, child_1, child_2;
	bool cond_1, cond_2;

	while (true)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		cond_1 = child_1 >= size;
		cond_2 = child_2 >= size;

		if (cond_1 && cond_2)
		{
			break;
		}
		else if (cond_2)
		{
			if (arr[child_1] > arr[index])
			{
				swap(arr + child_1, arr + index);
				index = child_1;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (arr[child_2] >= arr[child_1])
			{
				if (arr[child_2] > arr[index])
				{
					swap(arr + child_2, arr + index);
					index = child_2;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (arr[child_1] > arr[index])
				{
					swap(arr + child_1, arr + index);
					index = child_1;
				}
				else
				{
					break;
				}
			}
		}
	}

	return arr;
}

int *sort(int *numbers, int size)
{
	int i, heap_start = 0, border = 1;

	numbers = make_heap(numbers, size);

	for (i = 0; i < size - 1; i++)
	{
		swap(numbers, numbers + size - border);
		numbers = fix_heap(numbers, size - border);
		border++;
	}

	return numbers;
}

int main()
{

	return 0;
}