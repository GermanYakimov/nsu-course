#pragma once

typedef struct AVL_node_ {
	void *key;
	struct AVL_node_ *right;
	struct AVL_node_ *left;
	size_t height;
} node;


node *remove_AVL(node *root, void *key, size_t size, int compare(void*, void*));

node *insert_AVL(node *root, void *key, size_t size, int compare(void*, void*));

int is_in_tree_AVL(node *root, void *key, int compare(void*, void*));

void print_tree(node *root, void print(void*));

void delete_tree(node *root);

int nodes_number(node *root);

void print_data_on_level(node *root, int level, void print(void*));
