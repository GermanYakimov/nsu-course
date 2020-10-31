#pragma once
#include <set>
#include <iostream>

#include "Node.h"

using namespace std;

template <typename K, typename V>
class List
{
	Node<K, V>* head;
	size_t elements;

	Node<K, V>* get_pointer(K key)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (tmp->key == key)
			{
				return tmp;
			}
			tmp = tmp->next;
		}

		return nullptr;
	}

	void append(K key, V value)
	{
		Node<K, V>* new_element = new Node<K, V>(key, value, this->head);
		this->head = new_element;
	}

public:
	List() : head(nullptr), elements(0) {}

	List(K key, V value) : head(new Node<K, V>(key, value, nullptr)), elements(1) {}

	size_t size()
	{
		return this->elements;
	}

	V get(K key)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (tmp->key == key)
			{
				return tmp->value;
			}
			tmp = tmp->next;
		}

		throw invalid_argument("Node with given key doesn't exist.");
	}

	set<V> unique_elements()
	{
		set<V> result;
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			result.insert(tmp->value);
			tmp = tmp->next;
		}

		return result;
	}

	void rewrite_or_add(K key, V value)
	{
		Node<K, V>* element = this->get_pointer(key);

		if (!element)
		{
			this->append(key, value);
			this->elements++;
		}
		else
		{
			element->value = value;
		}
	}

	void remove(K key)
	{
		Node<K, V>* current = this->head;
		Node<K, V>* prev = nullptr;

		while (current)
		{
			if (current->key == key)
			{
				if (prev)
				{
					prev->next = current->next;
				}
				else
				{
					this->head = current->next;
				}

				this->elements--;
				delete current;
				return;
			}

			prev = current;
			current = current->next;
		}

		//throw invalid_argument("Node with given key doesn't exist.");
	}

	~List()
	{
		Node<K, V>* prev;
		Node<K, V>* current = this->head;

		while (current)
		{
			prev = current;
			current = current->next;

			delete prev;
		}
	}
};
