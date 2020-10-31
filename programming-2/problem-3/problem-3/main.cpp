#include <iostream>
#include <vector>
#include <set>
#include <fstream>
using namespace std;

template <typename K, typename V>
class List;

template <typename K, typename V>
class Node
{
	K key;
	V value;
	Node* next;

	friend class List<K, V>;

	Node(K k, V v, Node* next) : key(k), value(v), next(next) {}
};

template <typename K, typename V>
class List
{
	Node<K, V>* head;
	size_t elements;

	Node<K, V>* get_pointer(K key)
	{
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			if (tmp->key == key)
			{
				return tmp;
			}
			tmp = tmp->next;
		}

		return nullptr;
	}

	void append(K key, V value)
	{
		Node<K, V>* new_element = new Node<K, V>(key, value, this->head);
		this->head = new_element;
	}

public:
	List() : head(nullptr), elements(0) {}

	List(K key, V value): head(new Node<K, V>(key, value, nullptr)), elements(1) {}

	size_t size()
	{
		return this->elements;
	}

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

	set<V> unique_elements()
	{
		set<V> result;
		Node<K, V>* tmp = this->head;

		while (tmp)
		{
			result.insert(tmp->value);
			tmp = tmp->next;
		}

		return result;
	}

	void rewrite_or_add(K key, V value)
	{
		Node<K, V>* element = this->get_pointer(key);

		if (!element)
		{
			this->append(key, value);
			this->elements++;
		}
		else
		{
			element->value = value;
		}
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
				}
				else
				{
					this->head = current->next;
				}

				this->elements--;
				delete current;
				return;
			}

			prev = current;
			current = current->next;
		}

		//throw invalid_argument("Node with given key doesn't exist.");
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

template <typename K, typename V>
void do_commands(istream& input, size_t commands_number)
{
	HashMap<K, V> dict;
	K key;
	V value;
	char command;

	for (size_t i = 0; i < commands_number; i++)
	{
		input >> command;

		switch (command)
		{
		case 'R':
			input >> key;
			dict.remove(key);
			break;
		case 'A':
			input >> key;
			input >> value;
			dict.add(key, value);
			break;
		default:
			break;
		}
	}

	cout << dict.size() << " " << dict.unique_elements() << endl;
}

int main()
{
	ifstream input("input.txt");
	char k_type, v_type;
	size_t commands_number;
	input >> k_type >> v_type;
	input >> commands_number;
	//cout << k_type << " " << v_type;

	switch (k_type)
	{
	case 'I':
		switch (v_type)
		{
		case 'I':
			do_commands<int, int>(input, commands_number);
			break;
		case 'D':
			do_commands<int, double>(input, commands_number);
			break;
		case 'S':
			do_commands<int, string>(input, commands_number);
			break;
		default:
			throw invalid_argument("Incorrect value type.");
		}
		break;
	case 'D':
		switch (v_type)
		{
		case 'I':
			do_commands<double, int>(input, commands_number);
			break;
		case 'D':
			do_commands<double, double>(input, commands_number);
			break;
		case 'S':
			do_commands<double, string>(input, commands_number);
			break;
		default:
			throw invalid_argument("Incorrect value type.");
		}
		break;
	case 'S':
		switch (v_type)
		{
		case 'I':
			do_commands<string, int>(input, commands_number);
			break;
		case 'D':
			do_commands<string, double>(input, commands_number);
			break;
		case 'S':
			do_commands<string, string>(input, commands_number);
			break;
		default:
			throw invalid_argument("Incorrect value type.");
		}
		break;
	default:
		throw invalid_argument("Incorrect key type.");
	}

	input.close();

	return EXIT_SUCCESS;
}
