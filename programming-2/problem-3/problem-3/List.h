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

	size_t get_index_by_key(K key)
	{
		Node<K, V>* tmp = this->head;
		size_t result = 0;

		while (tmp)
		{
			if (tmp->key == key)
			{
				return result;
			}

			tmp = tmp->next;
			result++;
		}

		throw invalid_argument("Element with given key doesn't exist.");
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

	size_t rewrite(K key, V value)
	{
		Node<K, V>* element = this->get_pointer(key);

		if (!element)
		{
			this->append(key, value);
			return 0;
		}
		else
		{
			element->value = value;
			return 1;
		}
	}

	size_t remove_all(K key)
	{
		size_t deleted_elements = 0;

		while (this->get_pointer(key))
		{
			deleted_elements += this->remove(key);
		}

		return deleted_elements;
	}

	bool exists(K key, V value) const
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (key == tmp->key && value == tmp->value)
			{
				return true;
			}
			tmp = tmp->next;
		}

		return false;
	}

	bool empty() const
	{
		return this->head == nullptr;
	}

	size_t remove(K key)
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
				return 1;
			}

			prev = current;
			current = current->next;
		}

		return 0;
	}

	~List()
	{
		Node<K, V>* prev = nullptr;
		Node<K, V>* current = this->head;

		while (current)
		{
			prev = current;
			current = current->next;

			delete prev;
		}
	}
};
