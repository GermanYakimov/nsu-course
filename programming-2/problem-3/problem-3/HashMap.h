#pragma once
#include <vector>
#include <iostream>
#include <iterator>

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

	class Iterator
	{
		K key;
		typename vector<List<K, V>*>::iterator iter;
		typename vector<List<K, V>*>::iterator end;

	public:
		Iterator(K k, typename vector<List<K, V>*>::iterator i, typename vector<List<K, V>*>::iterator e) : key(k), iter(i), end(e) {}

		friend const Iterator& operator++(Iterator& it, int)
		{
			K next;
			Iterator it_copy = it;

			try
			{
				next = (*(it.iter))->get_next_key(it.key);
				it = Iterator(it.key, it.iter, it.end);
				return it_copy;
			}
			catch (exception)
			{
				for (typename vector<List<K, V>*>::iterator i = it.iter + 1; i != it.end; i++)
				{
					if ((*i) && ((*i)->size() > 0))
					{
						it = Iterator((*i)->get_head_key(), i, it.end);
						return it_copy;
					}
				}
			}
		}

		friend bool operator!=(const Iterator& one, const Iterator& two)
		{
			return one.iter != two.iter || one.key != two.key;
		}

		K get_key()
		{
			return this->key;
		}

		V get_value()
		{
			return (*(this->iter))->get(this->key);
		}
	};

	Iterator begin()
	{
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i] && this->data[i]->size() > 0)
			{
				return Iterator(this->data[i]->get_head_key(), this->data.begin() + i, this->data.end());
			}
		}
	}

	Iterator end()
	{
		size_t index;
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i] && (this->data[i]->size() > 0))
			{
				index = i;
			}
		}

		return Iterator(this->data[index]->get_tail_key(), this->data.begin() + index, this->data.end());
	}

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
