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

	if (!new_node)
	{
		printf("Can't allocate memory for new node.");
		return NULL;
	}

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

void swap_heap_nodes(Node **one, Node **two)
{
	int one_index = (*one)->heap_index, two_index = (*two)->heap_index;
	swap(one, two);
	(*one)->heap_index = one_index;
	(*two)->heap_index = two_index;
}

Node **sift_down(Node **heap, int index, size_t size, int(*compare)(Node*, Node*))
{
	int child_1, child_2;

	while (1)
	{
		child_1 = 2 * index + 1;
		child_2 = 2 * index + 2;

		if (child_1 >= (int)size && child_2 >= (int)size)
		{
			return heap;
		}
		else if (child_2 >= (int)size)
		{
			if (compare(heap[child_1], heap[index]) > 0)
			{
				swap_heap_nodes(heap + child_1, heap + index);
				index = child_1;
			}
			else
			{
				return heap;
			}
		}
		else
		{
			if (compare(heap[child_2], heap[child_1]) >= 0)
			{
				if (compare(heap[child_2], heap[index]) > 0)
				{
					swap_heap_nodes(heap + child_2, heap + index);
					index = child_2;
				}
				else
				{
					return heap;
				}
			}
			else
			{
				if (compare(heap[child_1], heap[index]) > 0)
				{
					swap_heap_nodes(heap + child_1, heap + index);
					index = child_1;
				}
				else
				{
					return heap;
				}
			}
		}
	}
}

Node **sift_up(Node *node, Node **heap, int(*compare)(Node*, Node*))
{
	int node_index = node->heap_index, parent_index;
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
			swap_heap_nodes(heap + parent->heap_index, heap + node->heap_index);
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
		v->dist = u->dist + weight;
		return sift_up(v, heap, compare);
	}

	return heap;
}

void dijkstra(int target, int size, Node *heap_base, Node **heap, list_node **adjacency_list)
{
	Node *u;
	int border = 0;
	list_node *head;
	
	heap_base[0].dist = 0;

	while (heap[0]->number != target)
	{
		u = heap[0];

		border++;
		swap_heap_nodes(heap, heap + size - border);
		heap = sift_down(heap, 0, size - border, compare);

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

	if (!adjacency_list || !heap_base || !heap || !p)
	{
		printf("Can't allocate memory");
		return -1;
	}

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

		if (!adjacency_list[from])
		{
			return -1;
		}

		adjacency_list[to] = add_node(adjacency_list[to], heap[from], weight);

		if (!adjacency_list[to])
		{
			return -1;
		}
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
