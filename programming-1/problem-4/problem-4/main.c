#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "trees.h"


void print(char *str)
{
	printf("%s", str);
}

int substr_count(char *str, char *substr)
{
	int count = 0;
	char* p = strstr(str, substr);

	while (p)
	{
		count++;
		p = strstr(p + 1, substr);
	}

	return count;
}

node_AVL *add_words_to_tree(char *str, node_AVL *root)
{
	int words_count = substr_count(str, " ") + 1;
	char *p = str, *nearest_space, *tmp = (char*)malloc(words_count * sizeof(char));

	if (!tmp)
	{
		printf("Can't allocate memory for temporary string");
		return NULL;
	}

	for (int i = 0; i < words_count; i++)
	{
		nearest_space = strstr(p, " ");

		if (!nearest_space)
		{
			if (!isalnum(p[strlen(p) - 1]))
			{
				p[strlen(p) - 1] = '\0';
			}
			root = insert_AVL(root, p, (strlen(p) + 1) * sizeof(char), strcmp);

			if (!is_in_tree_AVL(root, p, strcmp))
			{
				printf("Can't insert node");
				delete_tree(root);
				return NULL;
			}
		}
		else
		{
			memcpy(tmp, p, (nearest_space - p) * sizeof(char));
			tmp[nearest_space - p] = '\0';
			if (!isalnum(tmp[nearest_space - p - 1]))
			{
				tmp[nearest_space - p - 1] = '\0';
			}

			root = insert_AVL(root, tmp, (strlen(tmp) + 1) * sizeof(char), strcmp);

			if (!is_in_tree_AVL(root, tmp, strcmp))
			{
				printf("Can't insert node");
				delete_tree(root);
				return NULL;
			}

			p = nearest_space + 1;
		}
	}

	return root;
}

node_AVL *delete_words_from_tree(char **words, size_t words_num, node_AVL *root)
{
	size_t current_height;

	for (size_t i = 0; i < words_num; i++)
	{
		current_height = root->_height;
		root = remove_AVL(root, words[i], sizeof(words[i]), strcmp);

		if (current_height - root->_height > 1)
		{
			printf("Can't remove node from tree");
			return NULL;
		}
	}

	return root;
}

char *read_str(FILE *in)
{
	const size_t start_size = 1024;
	const size_t portion = 256;

	size_t current_size = 0;
	size_t current_max_size = start_size;
	char symbol;

	char *str = (char*)malloc(start_size * sizeof(char));

	if (!str)
	{
		printf("Can't allocate memory for string");
		return NULL;
	}

	while (1)
	{
		symbol = fgetc(in);

		if (symbol == '\n' || symbol == EOF)
		{
			str[current_size] = '\0';
			return str;
		}

		if (current_size + 1 == current_max_size)
		{
			str = (char*)realloc(str, current_max_size + portion);

			if (!str)
			{
				printf("Can't reallocate memory for string");
				return NULL;
			}

			current_max_size += portion;
		}

		str[current_size] = symbol;
		current_size++;
	}
}

int main()
{
	node_AVL *root = NULL;

	FILE *input = fopen("input.txt", "r");

	read_str(input);
	char *str = read_str(input), *tmp;
	read_str(input);

	int level, index = 0;
	int words_count = substr_count(str, " ") + 1;

	char **words_to_delete = (char**)malloc(words_count * sizeof(char*));

	if (!words_to_delete)
	{
		printf("Can't allocate memory for words");
		return -1;
	}

	while (1)
	{
		tmp = read_str(input);

		if (strcmp(tmp, "LEVEL:"))
		{
			words_to_delete[index] = tmp;
			index++;
		}
		else
		{
			break;
		}
	}
	free(tmp);

	fscanf(input, "%d", &level);
	fclose(input);

	root = add_words_to_tree(str, root);

	if (!root)
	{
		return -1;
	}

	print_tree(root, print);

	root = delete_words_from_tree(words_to_delete, index, root);
	printf("%d\n", nodes_number(root));

	print_data_on_level(root, level, print);

	delete_tree(root);
}
