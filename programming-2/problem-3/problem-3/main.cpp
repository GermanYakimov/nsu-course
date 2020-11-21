#include <iostream>
#include <vector>
#include <set>
#include <fstream>

#include "HashMap.h"
#include "MultiHashMap.h"
#include "Matrix.h"

using namespace std;

template <typename K, typename V>
void execute_commands(istream& input, size_t commands_number)
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

	cout << map.size() << " " << map.unique_elements() << endl;
}

template <typename T>
void execute_commands_depending_on_value_type(istream& input, size_t commands_number, char v_type)
{
	switch (v_type)
	{
	case 'I':
		execute_commands<T, int>(input, commands_number);
		break;
	case 'D':
		execute_commands<T, double>(input, commands_number);
		break;
	case 'S':
		execute_commands<T, string>(input, commands_number);
		break;
	default:
		throw invalid_argument("Incorrect value type.");
	}
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
		execute_commands_depending_on_value_type<int>(input, commands_number, v_type);
		break;
	case 'D':
		execute_commands_depending_on_value_type<double>(input, commands_number, v_type);
		break;
	case 'S':
		execute_commands_depending_on_value_type<string>(input, commands_number, v_type);
		break;
	default:
		throw invalid_argument("Incorrect key type.");
	}

	input.close();

	return EXIT_SUCCESS;
}
