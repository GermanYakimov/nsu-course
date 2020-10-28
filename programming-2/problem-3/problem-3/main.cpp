#include <iostream>
using namespace std;


template <typename K, typename V>
class Node
{
	K key;
	V value;
	Node* next;

	friend class List;

	Node(K k, V v, Node* next) : key(k), value(v), next(next) {}
};

template <typename K, typename V>
class List
{
	Node<K, V>* head;

public:
	List() : head(nullptr) {}

	List(K key, V value): head(new Node<K, V>(key, value, nullptr)) {}

	V get(K key)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (tmp->key == key)
			{
				return tmp->value;
			}
			tmp = tmp->next;
		}

		throw invalid_argument("Node with given key doesn't exist.");
	}

	void append(K key, V value)
	{
		Node<K, V>* new_element = new Node<K, V>(key, value, this->head);
		this->head = new_element;
	}

	void remove(K key)
	{
		Node<K, V>* current = this->head;
		Node<K, V>* prev = nullptr;

		while (current)
		{
			if (current->key == key)
			{
				if (prev)
				{
					prev->next = current->next;
					delete current;
				}

			}

			prev = current;
			current = current->next;
		}

		throw invalid_argument("Node with given key doesn't exist.");
	}

	~List()
	{
		Node<K, V>* prev;
		Node<K, V>* current = this->head;

		while (current)
		{
			prev = current;
			current = current->next;
			delete prev;
		}
	}
};

template <typename K, typename V>
class HashMap
{
	size_t get_index(K key)
	{

	}

public:
	V get(K key)
	{

	}

	void remove(K key)
	{

	}

	void add(K key, V value)
	{

	}
};
