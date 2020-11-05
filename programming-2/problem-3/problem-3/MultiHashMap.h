#pragma once
#include <vector>

#include "List.h"

using namespace std;

template <typename K, typename V>
class MultiHashMap
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
		//size_t current_size = this->size();
		//vector<pair<K, V>> tmp(current_size);

		//for (const auto& element : map)
		//{
		//	tmp.push_back(make_pair(element.key(), element.value()));
		//}
		//this->data.clear();
		//this->data.resize(this->data.size() + portion);

		//for (size_t i = 0; i < current_size; i++)
		//{
		//	this->add(tmp[i].first, tmp[i].second);
		//}
	}

public:
	class Iterator;

	MultiHashMap() : load_factor(0.7)
	{
		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	MultiHashMap(double lfactor)
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

	Iterator begin() {}

	Iterator end() {}

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

	void remove(K key) {}

	void add(K key, V value) {}

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

	~MultiHashMap()
	{
		this->clear();
	}
};
