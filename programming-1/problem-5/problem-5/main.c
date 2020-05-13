#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define UNDEFINED -1

typedef struct list_node_ {
	int graph_node_number, weight;
	struct list_node_ *next;
} list_node;

int *p, *d, *q, *heap;
list_node **adjacency_list;

list_node *add_node(list_node *head, int to, int weight)
{
	list_node *new_node = (list_node*)malloc(sizeof(list_node));
	new_node->graph_node_number = to;
	new_node->weight = weight;
	new_node->next = head;

	return new_node;
}

void free_list(list_node *head)
{
	list_node *tmp = head, *prev;

	while (tmp)
	{
		prev = tmp;
		tmp = tmp->next;

		free(prev);
	}
}

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int compare(int a, int b)
{
	return d[b] - d[a];
}

int *make_heap(int *arr, size_t num, int(*compare)(int, int))
{
	int i, index, parent;

	for (i = 0; i < (int)num; i++)
	{
		index = i;

		while (index != 0)
		{
			parent = (index - 1) / 2;

			if (0 <= parent <= (int)num && compare(arr[index], arr[parent]) <= 0)
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

int *fix_heap(int *arr, size_t num, int(*compare)(int, int))
{
	int index = 0, child_1, child_2;

	while (1)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		if (child_1 >= (int)num && child_2 >= (int)num)
		{
			return arr;
		}
		else if (child_2 >= (int)num)
		{
			if (compare(arr[child_1], arr[index]) > 0)
			{
				swap(arr + child_1, arr + index);
				index = child_1;
			}
			else
			{
				return arr;
			}
		}
		else
		{
			if (compare(arr[child_2], arr[child_1]) >= 0)
			{
				if (compare(arr[child_2], arr[index]) > 0)
				{
					swap(arr + child_2, arr + index);
					index = child_2;
				}
				else
				{
					return arr;
				}
			}
			else
			{
				if (compare(arr[child_1], arr[index]) > 0)
				{
					swap(arr + child_1, arr + index);
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


void relax(int u, int v, int weight)
{
	if (d[v] > d[u] + weight)
	{
		d[v] = d[u] + weight;
		p[v] = u;
	}
}

int *decrease_key(int *heap, size_t size, int index, int key)
{
	d[index] = key;
	int parent_node_index = (index - 1) / 2;

	while (index != 0 && d[parent_node_index] > d[index])
	{
		swap(heap + index, heap + parent_node_index);
		index = parent_node_index;
		parent_node_index = (index - 1) / 2;
	}
}

int extract_min(int size)
{
	int result = UNDEFINED;

	for (int i = 0; i < size; i++)
	{
		if (!q[i])
		{
			result = i;
			break;
		}
	}

	if (result == UNDEFINED)
	{
		return result;
	}

	for (int i = result + 1; i < size; i++)
	{
		if (!q[i] && d[i] < d[result])
		{
			result = i;
		}
	}

	return result;
}

void dijkstra(int s, int target, int size)
{
	int u, border = 0, tmp;
	list_node *head;

	d[s] = 0;

	while (1)
	{
		tmp = extract_min(size);
		u = heap[0];
		border++;
		swap(heap, heap + (size - border));
		// heap = fix_heap(heap, size - border + 1, compare);

		if (heap[0] == u)
		{
			return;
		}

		if (u == UNDEFINED)
		{
			return;
		}

		q[u] = 1;

		head = adjacency_list[u];

		while (head)
		{
			if (!q[head->graph_node_number])
			{
				relax(u, head->graph_node_number, head->weight);
			}
			head = head->next;
		}

		if (u == target)
		{
			return;
		}

		// heap = make_heap(heap, size - border, compare);
		heap = fix_heap(heap, size - border + 1, compare);
	}
}

void print_path(FILE *out, int size, int start, int current)
{
	if (current == start)
	{
		fprintf(out, "%d ", current);
		return;
	}

	print_path(out, size, start, p[current]);
	fprintf(out, "%d ", current);
}

int main()
{
	int target, points, paths, from, to, weight;
	FILE *input = fopen("input.txt", "r");

	if (!input)
	{
		printf("Can't open input.txt");
		return -1;
	}

	fscanf(input, "%d %d %d", &target, &points, &paths);

	adjacency_list = (list_node**)calloc(points, sizeof(list_node*));

	for (int i = 0; i < paths; i++)
	{
		fscanf(input, "%d %d", &from, &to);
		fscanf(input, "%d", &weight);

		adjacency_list[from] = add_node(adjacency_list[from], to, weight);
		adjacency_list[to] = add_node(adjacency_list[to], from, weight);
	}

	fclose(input);

	p = (int*)malloc(points * sizeof(int));
	d = (int*)malloc(points * sizeof(int));
	q = (int*)calloc(points, sizeof(int));

	heap = (int*)malloc(points * sizeof(int));

	for (int i = 0; i < points; i++)
	{
		heap[i] = i;
	}

	if (!p || !d || !q)
	{
		printf("Can't allocate memory for p-array, d-array or q-array.");
		return -1;
	}

	for (int i = 0; i<points; i++)
	{
		p[i] = UNDEFINED;
		d[i] = INT_MAX;
	}

	heap = make_heap(heap, points, compare);

	dijkstra(0, target, points);

	FILE *output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	fprintf(output, "%d\n", d[target]);
	print_path(output, points, 0, target);

	fclose(output);

	for (int i = 0; i < points; i++)
	{
		free_list(adjacency_list[i]);
	}

	free(adjacency_list);

	free(p);
	free(q);
	free(d);
	free(heap);

	return 0;
}
