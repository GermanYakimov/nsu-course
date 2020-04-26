#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trees.h"


void _free_node(node_AVL *tree_node)
{
	if (tree_node && tree_node->key)
	{
		free(tree_node->key);
		free(tree_node);
	}
	else if (tree_node)
	{
		free(tree_node);
	}
}

node_AVL *_new_node(void *key, size_t size)
{
	node_AVL *result = (node_AVL*)malloc(sizeof(node_AVL));

	if (!result)
	{
		printf("Can't allocate memory for new node");
		return NULL;
	}

	result->_height = 1;
	result->_left = NULL;
	result->_right = NULL;
	result->key = malloc(size);

	if (!result->key)
	{
		printf("Can't allocate memory for node's key");
		_free_node(result);
		return NULL;
	}

	memcpy(result->key, key, size);

	return result;
}

size_t _height(node_AVL *tree_node)
{
	if (!tree_node)
	{
		return 0;
	}

	return tree_node->_height;
}

void _fix_height(node_AVL *tree_node)
{
	if (!tree_node)
	{
		return;
	}

	tree_node->_height = max(_height(tree_node->_left), _height(tree_node->_right)) + 1;
}

short _balance_factor(node_AVL *tree_node)
{
	if (!tree_node)
	{
		return 0;
	}

	return (short)((int)_height(tree_node->_right) - (int)_height(tree_node->_left));
}

node_AVL *_rotate_right_AVL(node_AVL *tree_node)
{
	if (!tree_node)
	{
		return NULL;
	}

	node_AVL *left_child = tree_node->_left;

	if (!left_child)
	{
		return tree_node;
	}

	tree_node->_left = left_child->_right;
	left_child->_right = tree_node;

	_fix_height(tree_node);
	_fix_height(left_child);

	return left_child;
}

node_AVL *_rotate_left_AVL(node_AVL *tree_node)
{
	if (!tree_node)
	{
		return NULL;
	}

	node_AVL *right_child = tree_node->_right;

	if (!right_child)
	{
		return tree_node;
	}

	tree_node->_right = right_child->_left;
	right_child->_left = tree_node;

	_fix_height(tree_node);
	_fix_height(right_child);

	return right_child;
}

node_AVL *_rebalance_AVL(node_AVL *tree_node)
{
	if (_balance_factor(tree_node) == 2)
	{
		if (_balance_factor(tree_node->_right) < 0)
		{
			tree_node->_right = _rotate_right_AVL(tree_node->_right);
		}
		
		return _rotate_left_AVL(tree_node);
	}
	else if (_balance_factor(tree_node) == -2)
	{
		if (_balance_factor(tree_node->_left) < 0)
		{
			tree_node->_left = _rotate_left_AVL(tree_node->_left);
		}

		return _rotate_right_AVL(tree_node);
	}

	return tree_node;
}

int is_in_tree_AVL(node_AVL *root, void *key, int compare(void*, void*))
{
	if (!root)
	{
		return 0;
	}

	if (!compare(root->key, key))
	{
		return 1;
	}

	if (compare(root->key, key) < 0)
	{
		return is_in_tree_AVL(root->_right, key, compare);
	}
	else
	{
		return is_in_tree_AVL(root->_left, key, compare);
	}
}


void print_tree(node_AVL *root, void print(void*, FILE*), FILE *file) 
{
	if (!root) 
	{
		fprintf(file, "x");
		return;
	}

	fprintf(file, "(");
	print(root->key, file);
	print_tree(root->_left, print, file);
	print_tree(root->_right, print, file);
	fprintf(file, ")");
}

void print_data_on_level(node_AVL *root, int level, void print(void*, FILE*), FILE *file)
{
	if (!root)
	{
		return;
	}

	if (level == 0)
	{
		print(root->key, file);
		fprintf(file, " ");
		return;
	}

	print_data_on_level(root->_left, level - 1, print, file);
	print_data_on_level(root->_right, level - 1, print, file);
}

int nodes_number(node_AVL *root)
{
	if (!root)
	{
		return 0;
	}

	return 1 + nodes_number(root->_right) + nodes_number(root->_left);
}

node_AVL *remove_AVL(node_AVL *root, void *key, size_t size, int compare(void*, void*))
{
	if (!is_in_tree_AVL(root, key, compare))
	{
		return root;
	}

	if (compare(key, root->key) < 0)
	{
		root->_left = remove_AVL(root->_left, key, size, compare);
		_fix_height(root->_left);
	}
	else if (compare(key, root->key) > 0)
	{
		root->_right = remove_AVL(root->_right, key, size, compare);
		_fix_height(root->_right);
	}
	else
	{
		node_AVL *left_child = root->_left;
		node_AVL *right_child = root->_right;

		_free_node(root);

		if (!right_child)
		{
			return left_child;
		}
		node_AVL *min, *min_prev;

		min = right_child;

		while (min->_left)
		{
			min_prev = min;
			min = min->_left;
		}

		if (min == right_child)
		{
			right_child->_left = left_child;
			_fix_height(right_child);
			return _rebalance_AVL(right_child);
		}

		min_prev->_left = NULL;
		_fix_height(min_prev);
		_fix_height(right_child);
		_fix_height(left_child);

		min->_left = left_child;
		min->_right = right_child;
		_fix_height(min);

		return _rebalance_AVL(min);
	}

	_fix_height(root);

	return _rebalance_AVL(root);
}

node_AVL *insert_AVL(node_AVL *root, void *key, size_t size, int compare(void*, void*))
{
	if (!root)
	{
		return _new_node(key, size);
	}

	if (is_in_tree_AVL(root, key, compare))
	{
		return root;
	}

	if (compare(key, root->key) < 0)
	{
		root->_left = insert_AVL(root->_left, key, size, compare);
	}
	else
	{
		root->_right = insert_AVL(root->_right, key, size, compare);
	}

	_fix_height(root);

	return _rebalance_AVL(root);
}

void delete_tree(node_AVL *root)
{
	if (!root)
	{
		return;
	}

	delete_tree(root->_right);
	delete_tree(root->_left);

	_free_node(root);
}
