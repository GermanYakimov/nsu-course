#include <vector>
#include <iostream>

#include "List.cpp"

using namespace std;

template <typename K, typename V>
class HashMap
{
	vector<List<K, V>*> data;
	hash<K> hasher;

	static const size_t default_size = 10000;
	static const size_t portion = 5000;

	size_t get_index(K key)
	{
		return this->hasher(key) % this->data.size();
	}

	bool need_to_resize()
	{
		return this->size() / this->data.size() > 0.7;
	}

	void resize()
	{
		this->data.resize(this->data.size() + portion);
	}

public:
	HashMap()
	{
		data.resize(default_size);
		fill(data.begin(), data.end(), nullptr);
	}

	size_t size()
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

	V get(K key)
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

	size_t unique_elements()
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

			if (this->need_to_resize())
			{
				this->resize();
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
