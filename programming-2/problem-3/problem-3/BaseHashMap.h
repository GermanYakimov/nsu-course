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

	bool time_to_rehash() const
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

	BaseHashMap() : load_factor(0.7), map_size(0)
	{
		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	BaseHashMap(double lfactor) : map_size(0)
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
		int list_index; // element index in the list
		typename vector<List<K, V>*>::iterator current_position;
		typename vector<List<K, V>*>::iterator end;

	public:
		Iterator(size_t idx, typename vector<List<K, V>*>::iterator i, typename vector<List<K, V>*>::iterator e) : list_index(idx), current_position(i), end(e) {}

		Iterator operator++(int)
		{
			if (this->list_index == -1)
			{
				throw out_of_range("Can't increment map iterator past the end.");
			}

			Iterator this_copy = *this;

			if (this->list_index < (int)(*(this->current_position))->size() - 1)
			{
				*this = Iterator(this->list_index + 1, this->current_position, this->end);
				return this_copy;
			}

			for (typename vector<List<K, V>*>::iterator i = this->current_position + 1; i != this->end; i++)
			{
				if (*i)
				{
					*this = Iterator(0, i, this->end);
					return this_copy;
				}
			}

			*this = Iterator(-1, this->end, this->end); // this is the end
			return this_copy;
		}

		Iterator& operator++()
		{
			(*this)++;
			return *this;
		}

		friend bool operator==(const Iterator& one, const Iterator& two)
		{
			return (one.current_position == two.current_position) && (one.list_index == two.list_index);
		}

		friend bool operator!=(const Iterator& one, const Iterator& two)
		{
			return !(one == two);
		}

		K key() const
		{
			if (this->list_index != -1 && *(this->current_position))
			{
				return (*(this->current_position))->get_key(this->list_index);
			}
			throw out_of_range("Can't dereference iterator over an empty collection.");
		}

		V value() const
		{
			if (this->list_index != -1 && *(this->current_position))
			{
				return (*(this->current_position))->get_value(this->list_index);
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
		// find first non-empty list
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				return Iterator(0, this->data.begin() + i, this->data.end());
			}
		}

		// collection is empty
		return Iterator(0, this->data.begin(), this->data.begin());
	}

	Iterator end()
	{
		// check that the collection has at least one element
		for (size_t i = 0; i < this->data.size(); i++)
		{
			if (this->data[i])
			{
				return Iterator(-1, this->data.end(), this->data.end());
			}
		}

		// collection is empty
		return Iterator(0, this->data.begin(), this->data.begin());
	}

	size_t size() const
	{
		return this->map_size;
	}

	Iterator get(K key)
	{
		size_t index = this->get_index(key);

		if (!this->data[index])
		{
			return this->end();
		}

		size_t list_idx = this->data[index]->get_index_by_key(key);

		return Iterator(list_idx, this->data.begin() + index, this->data.end());
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
