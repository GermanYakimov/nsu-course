#include <iostream>
#include <vector>
#include <set>
#include <fstream>

#include "HashMap.h"
using namespace std;

template <typename K, typename V>
void do_commands(istream& input, size_t commands_number)
{
	HashMap<K, V> map;
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
			map.remove(key);
			break;
		case 'A':
			input >> key;
			input >> value;
			map.add(key, value);
			break;
		default:
			break;
		}
	}

	for (typename HashMap<K, V>::Iterator iter = map.begin(); iter != map.end(); iter++)
	{
		cout << iter.key() << " " << iter.value() << endl;
	}

	typename HashMap<K, V>::Iterator e = map.end();

	cout << e.key() << " " << e.value() << endl;

	cout << map.size() << " " << map.unique_elements() << endl;
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


	//HashMap<string, int> map;

	//map.add("Ivanov", 1);
	//map.add("Petrova", 2);
	//map.add("Ivanov", 3);
	//map.add("Sidorov", 4);
	//map.add("Smith", 5);

	//HashMap<string, int>::Iterator iter = map.begin();

	//cout << iter.key() << iter.value() << endl;

	//for (HashMap<string, int>::Iterator iter = map.begin(); iter != map.end(); iter++)
	//{
	//	cout << iter.key() << iter.value() << endl;
	//}
	//vector<int> v = { 1, 2 };

	//cout << *(v.end()) << endl;

	//for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}


	return EXIT_SUCCESS;
}
