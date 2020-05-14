#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define UNDEFINED -1

int *p;

typedef struct node_ {
	int number;
	int dist;
	int heap_index;
	int marked;
} Node;

typedef struct list_node_ {
	Node *node;
	int weight;
	struct list_node_ *next;
} list_node;

list_node *add_node(list_node *head, Node *to, int weight)
{
	list_node *new_node = (list_node*)malloc(sizeof(list_node));
	new_node->node = to;
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

int compare(Node *one, Node *two)
{
	return two->dist - one->dist;
}

void swap(Node **one, Node **two)
{
	Node *tmp = *one;
	*one = *two;
	*two = tmp;
}

Node **make_heap(Node **arr, size_t num, int(*compare)(Node*, Node*))
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
				arr[index]->heap_index = index;
				arr[parent]->heap_index = parent;

				index = parent;
			}
		}
	}

	return arr;
}

Node **fix_heap(Node **arr, size_t num, int(*compare)(Node*, Node*))
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
				arr[child_1]->heap_index = child_1;
				arr[index]->heap_index = index;

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
					arr[child_2]->heap_index = child_2;
					arr[index]->heap_index = index;

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
					arr[child_1]->heap_index = child_1;
					arr[index]->heap_index = index;

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

Node **decrease_key(Node *node, Node **heap, int key, int(*compare)(Node*, Node*))
{
	int node_index = node->heap_index, parent_index;
	int tmp_1, tmp_2;
	node->dist = key;
	Node *parent;

	while (1)
	{
		if (node_index == 0)
		{
			break;
		}

		parent_index = (node_index - 1) / 2;

		parent = heap[parent_index];
		node = heap[node_index];

		if (compare(node, parent) > 0)
		{
			tmp_1 = parent->heap_index;
			tmp_2 = node->heap_index;

			swap(heap + parent->heap_index, heap + node->heap_index);
			heap[tmp_1]->heap_index = tmp_1;
			heap[tmp_2]->heap_index = tmp_2;

			node_index = parent_index;
		}
		else
		{
			break;
		}
	}

	return heap;
}

Node **relax(Node *u, Node *v, Node **heap, int weight)
{
	if (v->dist > u->dist + weight)
	{
		p[v->number] = u->number;
		return decrease_key(v, heap, u->dist + weight, compare);
	}

	return heap;
}

void dijkstra(int target, int size, Node *heap_base, Node **heap, list_node **adjacency_list)
{
	Node *u;
	int border = 0;
	list_node *head;
	
	heap_base[0].dist = 0;

	while (1)
	{
		u = heap[0];
		border++;
		swap(heap, heap + (size - border));
		heap[0]->heap_index = 0;
		heap[size - border]->heap_index = size - border;
		heap = fix_heap(heap, size - border, compare);

		if (heap[0] == u)
		{
			return;
		}

		u->marked = 1;

		head = adjacency_list[u->number];

		while (head)
		{
			if (!head->node->marked)
			{
				heap = relax(u, head->node, heap, head->weight);
			}
			head = head->next;
		}

		if (u->number == target)
		{
			return;
		}
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

	list_node **adjacency_list = (list_node**)calloc(points, sizeof(list_node*));
	Node *heap_base = (Node*)malloc(points * sizeof(Node));
	Node **heap = (Node**)malloc(points * sizeof(Node*));
	p = (int*)malloc(points * sizeof(int));

	for (int i = 0; i < points; i++)
	{
		heap[i] = heap_base + i;
		heap[i]->dist = INT_MAX;
		heap[i]->heap_index = i;
		heap[i]->number = i;
		heap[i]->marked = 0;

		p[i] = UNDEFINED;
	}

	for (int i = 0; i < paths; i++)
	{
		fscanf(input, "%d %d", &from, &to);
		fscanf(input, "%d", &weight);

		adjacency_list[from] = add_node(adjacency_list[from], heap[to], weight);
		adjacency_list[to] = add_node(adjacency_list[to], heap[from], weight);
	}

	fclose(input);

	dijkstra(target, points, heap_base, heap, adjacency_list);

	FILE *output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	fprintf(output, "%d\n", heap_base[target].dist);
	print_path(output, points, 0, target);

	fclose(output);

	free(p);
	for (int i = 0; i < points; i++)
	{
		free(adjacency_list[i]);
	}
	free(adjacency_list);
	free(heap_base);
	free(heap);

	return 0;
}
