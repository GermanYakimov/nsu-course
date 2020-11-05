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
			this->data[index]->remove_all(key);

			if (!this->data[index]->size())
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

			if (this->need_to_rehash())
			{
				this->rehash();
			}
		}
		else
		{
			this->data[index]->append(key, value);
		}
	}

};
