#pragma once
#include <vector>
#include <iostream>
#include <iterator>

#include "List.h"
#include "BaseHashMap.h"

using namespace std;

template <typename K, typename V>
class HashMap: public BaseHashMap<K, V>
{
public:
	HashMap() : BaseHashMap<K, V>() {}

	HashMap(double lfactor) : BaseHashMap(lfactor) {}

	virtual void remove(K key)
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

	virtual void add(K key, V value)
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
};
