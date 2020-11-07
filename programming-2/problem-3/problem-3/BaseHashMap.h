#pragma once

#include<vector>
#include <iostream>

#include "List.h"

using namespace std;

template <typename K, typename V>
class BaseHashMap
{
protected:
	vector<List<K, V>*> data;
	hash<K> hasher;

	static const size_t default_size = 10000;
	static const size_t portion = 5000;

	double load_factor;
	size_t map_size;

	size_t get_index(K key) const
	{
		return this->hasher(key) % this->data.size();
	}

	bool need_to_rehash() const
	{
		return (this->map_size / this->data.size()) > this->load_factor;
	}

	void rehash()
	{
		size_t current_size = this->map_size;
		vector<pair<K, V>> tmp;

		for (const auto& element : *this)
		{
			tmp.push_back(make_pair(element.key(), element.value()));
		}
		this->clear();
		this->data.resize(this->data.size() + portion);

		for (size_t i = 0; i < current_size; i++)
		{
			this->add(tmp[i].first, tmp[i].second);
		}
		this->map_size = current_size;
	}

public:

	BaseHashMap() : load_factor(0.7), array_size(default_size), map_size(0)
	{
		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	BaseHashMap(double lfactor) : array_size(default_size), map_size(0)
	{
		if (0 < lfactor <= 1)
		{
			this->load_factor = lfactor;
		}
		else
		{
			throw invalid_argument("Invalid load factor.");
		}

		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

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

	size_t size() const
	{
		return this->map_size;
	}

	V get(K key) const
	{
		size_t index = this->get_index(key);

		if (!this->data[index])
		{
			throw invalid_argument("Element with given key doesn't exist.");
		}

		return this->data[index]->get(key);
	}

	virtual void remove(K key) = 0;

	virtual void add(K key, V value) = 0;

	void clear()
	{
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				delete this->data[i];
				this->data[i] = nullptr;
			}
		}
		this->map_size = 0;
	}

	~BaseHashMap()
	{
		this->clear();
	}
};
