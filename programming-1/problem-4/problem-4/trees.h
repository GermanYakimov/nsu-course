#pragma once

typedef struct AVL_node_ {
	void *key;
	struct AVL_node_ *_right;
	struct AVL_node_ *_left;
	size_t _height;
} node_AVL;

typedef struct RB_node_
{
	void *key;
	struct RB_node_ *_right;
	struct RB_node_ *_left;
	size_t _height;
	char color;
} node_RB;


node_AVL *remove_AVL(node_AVL *root, void *key, size_t size, int compare(void*, void*));

node_AVL *insert_AVL(node_AVL *root, void *key, size_t size, int compare(void*, void*));

int is_in_tree_AVL(node_AVL *root, void *key, int compare(void*, void*));

void print_tree(node_AVL *root, void print(void*));

void delete_tree(node_AVL *root);

int nodes_number(node_AVL *root);

void print_data_on_level(node_AVL *root, int level, void print(void*));
