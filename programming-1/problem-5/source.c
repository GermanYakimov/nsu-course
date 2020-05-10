#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int *p, *d, *q;
int **matrix;

int **allocate_matrix(int n)
{
	int **matrix = (int**)malloc(n * sizeof(int*));
	
	if (!matrix)
	{
		return NULL;
	}

	for (int i = 0; i < n; i++)
	{
		matrix[i] = (int*)malloc(n * sizeof(int));

		if (!matrix[i])
		{
			for (int j = 0; j < i; j++)
			{
				free(matrix[j]);
			}
			free(matrix);
			
			return NULL;
		}

		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = -1;
		}
	}

	return matrix;
}

void free_matrix(int **matrix, int size)
{
	for(int i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void relax(int u, int v)
{
	if (d[v] > d[u] + matrix[u][v])
	{
		d[v] = d[u] + matrix[u][v];
		p[v] = u;
	}
}

int extract_min(int size)
{
	int result;

	for (int i = 0; i < size; i++)
	{
		if (!q[i])
		{
			result = i;
			break;
		}
	}

	for(int i = result + 1; i < size; i++)
	{
		if (!q[i] && d[i] < d[result])
		{
			result = i;
		}
	}

	if (!q[result])
	{
		return result;
	}
	else
	{
		return -1;
	}
}

void dijkstra(int s, int size)
{
	int u;

	p[s] = -1;
	d[s] = 0;

	while (1)
	{
		u = extract_min(size);

		if (u == -1)
		{
			break;
		}

		q[u] = 1;

		for (int i = 0; i < size; i++)
		{
			if (matrix[u][i] != -1)
			{
				relax(u, i);
			}
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
	
	int target, points, paths, from, to;
	FILE *input = fopen("input.txt", "r");
	
	if (!input)
	{
		printf("Can't open input.txt");
		return -1;
	} 

	fscanf(input, "%d %d %d", &target, &points, &paths);
	matrix = allocate_matrix(points);
	
	for(int i = 0; i < paths; i++)
	{
		fscanf(input, "%d %d", &from, &to);
		fscanf(input, "%d", &matrix[from][to]);
	}

	fclose(input);

	p = (int*)malloc(points*sizeof(int));
	d = (int*)malloc(points*sizeof(int));
	q = (int*)calloc(points, sizeof(int));

	if (!p || !d || !q)
	{
		printf("Can't allocate memory for p-array, d-array or q-array.");
		return -1;
	}

	for (int i=0; i<points; i++)
	{
		p[i] = -1;
		d[i] = INT_MAX;
	}

	dijkstra(0, points);

	FILE *output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	fprintf(output, "%d\n", d[target]);
	print_path(output, points, 0, target);

	fclose(output);

	free_matrix(matrix, points);
	free(p);
	free(q);
	free(d);

	return 0;
}
