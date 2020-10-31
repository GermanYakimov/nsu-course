#pragma once

template <typename K, typename V>
struct Node
{
	K key;
	V value;
	Node* next;

	Node(K k, V v, Node* next) : key(k), value(v), next(next) {}
};