#pragma once
#include <vector>

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
		size_t current_size = this->size();
		vector<pair<K, V>> tmp(current_size);

		for (const auto& element : map)
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
	class Iterator;

	Iterator begin() {}

	Iterator end() {}

	MultiHashMap() : load_factor(0.7) {}

	MultiHashMap(double lfactor) {}

	size_t size() const {}

	V get(K key) const {}

	void remove(K key) {}

	void add(K key, V value) {}

	void clear() {}

	~MultiHashMap() {}
};
