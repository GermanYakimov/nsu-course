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
		return (this->size() / this->data.size()) > this->load_factor;
	}

	void rehash()
	{
		size_t current_size = this->size();
		vector<pair<K, V>> tmp(current_size);

		for (const auto& element : *this)
		{
			tmp.push_back(make_pair(element.key(), element.value()));
		}
		this->data.clear();
		this->data.resize(this->data.size() + portion);

		for (size_t i = 0; i < current_size; i++)
		{
			this->add(tmp[i].first, tmp[i].second);
		}
	}

public:

	class Iterator
	{
		int list_index;
		typename vector<List<K, V>*>::iterator iter;
		typename vector<List<K, V>*>::iterator end;

	public:
		Iterator(size_t idx, typename vector<List<K, V>*>::iterator i, typename vector<List<K, V>*>::iterator e) : list_index(idx), iter(i), end(e) {}

		Iterator operator++(int)
		{
			Iterator this_copy = *this;
			if (this->list_index == -1)
			{
				throw out_of_range("Can't increment map iterator past the end.");
			}

			if (this->list_index < (int)(*(this->iter))->size() - 1)
			{
				*this = Iterator(this->list_index + 1, this->iter, this->end);
				return this_copy;
			}

			for (typename vector<List<K, V>*>::iterator i = this->iter + 1; i != this->end; i++)
			{
				if (*i)
				{
					*this = Iterator(0, i, this->end);
					return this_copy;
				}
			}

			*this = Iterator(-1, this->end, this->end);
			return this_copy;
		}

		Iterator& operator++()
		{
			if (this->list_index == -1)
			{
				throw out_of_range("Can't increment map iterator past the end.");
			}

			if (this->list_index < (int)(*(this->iter))->size() - 1)
			{
				*this = Iterator(this->list_index + 1, this->iter, this->end);
				return *this;
			}

			for (typename vector<List<K, V>*>::iterator i = this->iter + 1; i != this->end; i++)
			{
				if (*i)
				{
					*this = Iterator(0, i, this->end);
					return *this;
				}
			}

			*this = Iterator(-1, this->end, this->end);
			return *this;
		}

		friend bool operator==(const Iterator& one, const Iterator& two)
		{
			return one.iter == two.iter && one.list_index == two.list_index;
		}

		friend bool operator!=(const Iterator& one, const Iterator& two)
		{
			return one.iter != two.iter || one.list_index != two.list_index;
		}

		K key() const
		{
			if (*(this->iter))
			{
				return (*(this->iter))->get_key(this->list_index);
			}
			throw out_of_range("Can't dereference iterator over an empty collection.");
		}

		V value() const
		{
			if (this->list_index != -1 && *(this->iter))
			{
				return (*(this->iter))->get_value(this->list_index);
			}
			throw out_of_range("Can't dereference iterator over an empty collection.");
		}

		Iterator operator*()
		{
			return *this;
		}
	};


	Iterator begin()
	{
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i] && this->data[i]->size() > 0)
			{
				return Iterator(0, this->data.begin() + i, this->data.end());
			}
		}

		return Iterator(0, this->data.begin(), this->data.begin());
	}

	Iterator end()
	{
		size_t index = 0;

		for (size_t i = 1; i < this->data.size(); i++)
		{
			if (this->data[i] && (this->data[i]->size() > 0))
			{
				index = i;
			}
		}

		if (!index && !this->data[0])
		{
			return Iterator(0, this->data.begin(), this->data.begin());
		}

		return Iterator(-1, this->data.end(), this->data.end());
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

			if (!this->data[index]->size())
			{
				delete this->data[index];
				this->data[index] = nullptr;
			}
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
			this->data[index]->rewrite(key, value);
		}
	}

	void clear()
	{
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				delete this->data[i];
			}
			this->data[i] = nullptr;
		}
	}

	~HashMap()
	{
		this->clear();
	}
};
