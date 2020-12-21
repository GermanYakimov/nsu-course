#include <vector>
#include <string>

#include "DFA.h"

using namespace std;

int main()
{
	size_t n;
	vector<string> tests;
	string test;

	DFA fa;
	ifstream input("input.txt");

	fa = DFA::build_from_stream(input);
	input >> n;

	getline(input, test);
	for (size_t i = 0; i < n; i++)
	{
		getline(input, test);
		tests.push_back(test);
	}

	input.close();

	ofstream output("output.txt");

	for (auto test : tests)
	{
		output << fa.recognize(test) << endl;
	}
	output.close();

	// regexp test

	//DFA a;
	//a = DFA::build_from_regexp("((aaaabbbb)|((ababcd)*))");
	//cout << a.recognize("aaaabbbbababcdaaaabbbbababcd") << endl;
	//cout << a.recognize("ababcdababcdababcd") << endl;
	//cout << a.recognize("aaaabbbb") << endl;
	//cout << a.recognize("ababab") << endl;

	return EXIT_SUCCESS;
}