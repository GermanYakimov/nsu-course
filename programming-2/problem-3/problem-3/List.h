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

	Node<K, V>* get_pointer(K key) const
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

public:
	List() : head(nullptr), elements(0) {}

	List(K key, V value) : head(new Node<K, V>(key, value, nullptr)), elements(1) {}

	size_t size() const
	{
		return this->elements;
	}

	K get_key(size_t index)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (index == 0)
			{
				return tmp->key;
			}

			tmp = tmp->next;
			index--;
		}

		throw out_of_range("Given index is greater than list size.");
	}

	V get_value(size_t index)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (index == 0)
			{
				return tmp->value;
			}

			tmp = tmp->next;
			index--;
		}

		throw out_of_range("Given index is greater than list size.");
	}

	V get(K key) const
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

	set<V> unique_elements() const
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

	void append(K key, V value)
	{
		Node<K, V>* new_element = new Node<K, V>(key, value, this->head);
		this->head = new_element;
		this->elements++;
	}

	void rewrite(K key, V value)
	{
		Node<K, V>* element = this->get_pointer(key);

		if (!element)
		{
			this->append(key, value);
		}
		else
		{
			element->value = value;
		}
	}

	void remove_all(K key)
	{
		while (this->get_pointer(key))
		{
			this->remove(key);
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
