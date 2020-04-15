#pragma once

//typedef struct AVL_tree_node_ {
//	void *data;
//	AVL_node *right;
//	AVL_node *left;
//} AVL_node;
//
//typedef struct Red_Black_tree_node_ {
//	void *data;
//	RB_node *right;
//	RB_node *left;
//} RB_node;

typedef struct AVL_node_ {
	void *key;
	struct AVL_node_ *right;
	struct AVL_node_ *left;
	size_t height;
} node;


node *remove_AVL(node *root, char *key, size_t size, int compare(void*, void*));

node *insert_AVL(node *root, void *key, size_t size, int compare(void*, void*));

int is_in_tree_AVL(node *root, void *key, int compare(void*, void*));

void print_tree(node *root, void print(void*));

void delete_tree(node *root);
