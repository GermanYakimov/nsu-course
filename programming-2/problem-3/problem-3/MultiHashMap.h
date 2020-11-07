#pragma once
#include <vector>

#include "List.h"
#include "BaseHashMap.h"

using namespace std;

template <typename K, typename V>
class MultiHashMap: public BaseHashMap<K, V>
{
public:

	MultiHashMap() : BaseHashMap<K, V>() {}

	MultiHashMap(double lfactor): BaseHashMap<K, V>(lfactor) {}

	virtual void remove(K key)
	{
		size_t index = this->get_index(key);

		if (this->data[index])
		{
			this->map_size -= this->data[index]->remove_all(key);

			if (this->data[index]->empty())
			{
				delete this->data[index];
				this->data[index] = nullptr;
			}
		}
	}

	virtual void add(K key, V value)
	{
		size_t index = this->get_index(key);

		if (!this->data[index])
		{
			this->data[index] = new List<K, V>(key, value);
			this->map_size++;

			if (this->need_to_rehash())
			{
				this->rehash();
			}
		}
		else if (!this->data[index]->exists(key, value))
		{
			this->data[index]->append(key, value);
			this->map_size++;
		}
	}

	size_t slice_size(K key) const
	{
		size_t result = 0;

		for (auto element : *this)
		{
			if (element.key() == key)
			{
				result++;
			}
		}

		return result;
	}

	vector<V> slice(K key) const
	{
		vector<V> result;

		for (auto element : *this)
		{
			if (element.key() == key)
			{
				result.push_back(element.value());
			}
		}

		return result;
	}

};
