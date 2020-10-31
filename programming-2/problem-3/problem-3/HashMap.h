#pragma once
#include <vector>
#include <iostream>

#include "List.h"

using namespace std;

template <typename K, typename V>
class HashMap
{
	vector<List<K, V>*> data;
	hash<K> hasher;

	static const size_t default_size = 10000;
	static const size_t portion = 5000;
	double load_factor;

	size_t get_index(K key) const
	{
		return this->hasher(key) % this->data.size();
	}

	bool need_to_rehash() const
	{
		return this->size() / this->data.size() > this->load_factor;
	}

	void rehash()
	{
		this->data.resize(this->data.size() + portion);
	}

public:

	//class Iterator
	//{
	//	K key;
	//	vector<List<K, V>*>::iterator iter;

	//public:
	//	Iterator(K k, vector<List<K, V>*>::iterator i): key(k), iter(i) {}

	//	Iterator operator++()
	//	{
	//		K next;

	//		try 
	//		{
	//			next = *iter->get_next_key(key);
	//			return { next, this->iter };
	//		}
	//		catch
	//		{
	//			for (vector<List<K, V>*>::iterator i = this->iter; i != )
	//		}
	//	}
	//};

	//Iterator begin()
	//{
	//	for (size_t i = 0; i < this->data.size(); i++)
	//	{
	//		if (this->data[i] && this->data[i]->size() > 0)
	//		{
	//			return Iterator(this->data[i]->get_head_key());
	//		}
	//	}
	//}

	//Iterator end()
	//{

	//}

	HashMap() : load_factor(0.7)
	{
		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	HashMap(double lfactor)
	{
		if (0 < lfactor <= 1)
		{
			this->load_factor = lfactor;
		}
		else
		{
			throw invalid_argument("Invalid load factor");
		}

		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	size_t size() const
	{
		size_t result = 0;

		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				result += this->data[i]->size();
			}
		}

		return result;
	}

	V get(K key) const
	{
		size_t index = this->get_index(key);

		if (!this->data[index])
		{
			throw invalid_argument("Element with given key doesn't exist.");
		}
		else
		{
			return this->data[index]->get(key);
		}
	}

	void remove(K key)
	{
		size_t index = this->get_index(key);

		if (this->data[index])
		{
			this->data[index]->remove(key);
		}
	}

	size_t unique_elements() const
	{
		size_t result = 0;
		set<V> elements;
		set<V> elements_current;

		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				elements_current = this->data[i]->unique_elements();
				elements.insert(elements_current.begin(), elements_current.end());
			}
		}

		return elements.size();
	}

	void add(K key, V value)
	{
		size_t index = this->get_index(key);

		if (!this->data[index])
		{
			this->data[index] = new List<K, V>(key, value);

			if (this->need_to_rehash())
			{
				this->rehash();
			}
		}
		else
		{
			this->data[index]->rewrite_or_add(key, value);
		}
	}

	~HashMap()
	{
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				delete this->data[i];
			}
		}
	}
};
