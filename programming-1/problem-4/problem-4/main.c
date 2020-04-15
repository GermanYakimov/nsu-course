#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "trees.h"

void print(int *a)
{
	printf("%d", *a);
}

int greater(int *a, int *b)
{
	return *a - *b;
}

int main()
{
	int num, tmp;
	printf("numbers count: ");
	scanf("%d", &num);
	node *root = NULL;

	for (int i = 0; i < num; i++)
	{
		printf("value: ");
		scanf("%d", &tmp);

		root = insert_AVL(root, &tmp, sizeof(int), greater);
		print_tree(root, print);
		printf("\nheight: %lu", root->height);
		printf("\n");
	}

	printf("numbers to remove count: ");
	scanf("%d", &num);

	for (int i = 0; i < num; i++)
	{
		printf("value: ");
		scanf("%d", &tmp);

		root = remove_AVL(root, &tmp, sizeof(int), greater);
		print_tree(root, print);
		printf("\nheight: %lu", root->height);
		printf("\n");
	}

	delete_tree(root);
}
