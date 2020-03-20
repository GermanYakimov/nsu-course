#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

vector<int> make_heap(vector<int> arr, int border)
{
	int i, index, parent, size = arr.size() - border;

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
				swap(arr[index], arr[parent]);
				index = parent;
			}
		}
	}

	return arr;
}

vector<int> fix_heap(vector<int> arr, int border)
{
	int index = 0, child_1, child_2;
	bool cond_1, cond_2;

	while (true)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		cond_1 = child_1 >= arr.size() - border;
		cond_2 = child_2 >= arr.size() - border;

		if (cond_1 && cond_2)
		{
			break;
		}
		else if (cond_2)
		{
			if (arr[child_1] > arr[index])
			{
				swap(arr[child_1], arr[index]);
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
					swap(arr[child_2], arr[index]);
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
					swap(arr[child_1], arr[index]);
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

vector<int> remove_top_item(vector<int> heap)
{
	heap[0] = *(heap.end() - 1);
	heap.pop_back();

	int index, child_1, child_2, size = heap.size(), swap_child;
	index = 0;

	while (true)
	{
		if (index >= size)
		{
			break;
		}

		child_1 = index * 2 + 1;
		child_2 = child_1 + 1;

		if (child_1 >= size)
		{
			child_1 = index;
		}

		if (child_2 >= size)
		{
			child_2 = index;
		}

		if (heap[index] >= heap[child_1] && heap[index] >= heap[child_2])
		{
			break;
		}

		if (heap[child_1] > heap[child_2])
		{
			swap_child = child_1;
		}
		else
		{
			swap_child = child_2;
		}

		swap(heap[index], heap[swap_child]);
		index = swap_child;
	}

	return heap;
}

vector<int> sort(vector<int> numbers)
{
	int i, size = numbers.size();
	int heap_start = 0;
	int border = 0;

	numbers = make_heap(numbers, border);
	border++;

	for (i = 0; i < size - 1; i++)
	{
		swap(numbers[0], numbers[numbers.size() - border]);
		numbers = fix_heap(numbers, border);
		border++;
	}

	return numbers;
}

int main()
{
	srand(time(0));
	int i, j, len;
	cin >> len;

	vector<int> numbers(len);

	for (i = 0; i < len; i++)
	{
		numbers[i] = rand() % 1000;
		cout << numbers[i] << " ";
	}
	cout << endl << endl;

	numbers = sort(numbers);

	for (j = 0; j < numbers.size(); j++)
	{
		cout << numbers[j] << " ";
	}
	cout << endl;

	return 0;
}