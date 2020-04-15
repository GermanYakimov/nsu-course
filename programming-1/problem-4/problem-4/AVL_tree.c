#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trees.h"


node *_new_node(void *key, size_t size)
{
	node *result = (node*)malloc(sizeof(node));
	result->height = 1;
	result->left = NULL;
	result->right = NULL;
	result->key = malloc(size);

	memcpy(result->key, key, size);

	return result;
}

size_t _height(node *tree_node)
{
	if (!tree_node)
	{
		return 0;
	}

	return tree_node->height;
}

void _fix_height(node *tree_node)
{
	if (!tree_node)
	{
		return;
	}

	tree_node->height = max(_height(tree_node->left), _height(tree_node->right)) + 1;
}

short _balance_factor(node *tree_node)
{
	if (!tree_node)
	{
		return 0;
	}

	return (short)((int)_height(tree_node->right) - (int)_height(tree_node->left));
}

node *_rotate_right_AVL(node *tree_node)
{
	if (!tree_node)
	{
		return tree_node;
	}

	node *left_child = tree_node->left;


	if (!left_child)
	{
		return tree_node;
	}

	tree_node->left = left_child->right;
	left_child->right = tree_node;

	_fix_height(tree_node);
	_fix_height(left_child);

	return left_child;
}

node *_rotate_left_AVL(node *tree_node)
{
	if (!tree_node)
	{
		return tree_node;
	}

	node *right_child = tree_node->right;

	if (!right_child)
	{
		return tree_node;
	}

	tree_node->right = right_child->left;
	right_child->left = tree_node;

	_fix_height(tree_node);
	_fix_height(right_child);

	return right_child;
}

node *_rebalance_AVL(node *tree_node)
{
	if (_balance_factor(tree_node) == 2)
	{
		if (_balance_factor(tree_node->right) < 0)
		{
			tree_node->right = _rotate_right_AVL(tree_node->right);
		}
		
		return _rotate_left_AVL(tree_node);
	}
	else if (_balance_factor(tree_node) == -2)
	{
		if (_balance_factor(tree_node->left) < 0)
		{
			tree_node->left = _rotate_left_AVL(tree_node->left);
		}

		return _rotate_right_AVL(tree_node);
	}

	return tree_node;
}

int is_in_tree_AVL(node *root, void *key, int compare(void*, void*))
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
		return is_in_tree_AVL(root->right, key, compare);
	}
	else
	{
		return is_in_tree_AVL(root->left, key, compare);
	}
}

void print_tree(node *root, void print(void*))
{
	if (!root)
	{
		printf("x");
	}
	else
	{
		print(root->key);
		printf("(");
		print_tree(root->left, print);
		print_tree(root->right, print);
		printf(")");
	}
}

node *remove_AVL(node *root, void *key, size_t size, int compare(void*, void*))
{
	if (!is_in_tree_AVL(root, key, compare))
	{
		return root;
	}

	if (compare(key, root->key) < 0)
	{
		root->left = remove_AVL(root->left, key, size, compare);
		_fix_height(root->left);
	}
	else if (compare(key, root->key) > 0)
	{
		root->right = remove_AVL(root->right, key, size, compare);
		_fix_height(root->right);
	}
	else
	{
		node *left_child = root->left;
		node *right_child = root->right;

		free(root);

		if (!right_child)
		{
			return left_child;
		}
		node *min, *min_prev;

		min = right_child;

		while (min->left)
		{
			min_prev = min;
			min = min->left;
		}

		if (min == right_child)
		{
			right_child->left = left_child;
			_fix_height(right_child);
			return _rebalance_AVL(right_child);
		}

		node *new_root = _new_node(min->key, size);
		new_root->left = left_child;
		new_root->right = right_child;

		min_prev->left = NULL;
		_fix_height(min_prev);
		free(min);

		_fix_height(new_root);

		return _rebalance_AVL(new_root);
	}

	_fix_height(root);
	return _rebalance_AVL(root);
}

node *insert_AVL(node *root, void *key, size_t size, int compare(void*, void*))
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
		root->left = insert_AVL(root->left, key, size, compare);
	}
	else
	{
		root->right = insert_AVL(root->right, key, size, compare);
	}

	_fix_height(root);
	return _rebalance_AVL(root);
}

void delete_tree(node *root)
{
	if (!root)
	{
		return;
	}

	delete_tree(root->right);
	delete_tree(root->left);

	free(root->key);
	free(root);
}
