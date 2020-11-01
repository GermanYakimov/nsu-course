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

	for (const auto &element : map)
	{
		cout << element.key() << " " << element.value() << endl;
	}

	//typename HashMap<K, V>::Iterator e = map.end();

	//cout << e.key() << " " << e.value() << endl;

	cout << map.size() << " " << map.unique_elements() << endl;
}

int main()
{
	ifstream input("input.txt");
	char k_type, v_type;
	size_t commands_number;
	input >> k_type >> v_type;
	input >> commands_number;

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
