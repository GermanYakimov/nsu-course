#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "expressions.h"

int main()
{
	ifstream input("input.txt");
	string exp_source;
	input >> exp_source;
	input.close();

	Expression* exp = build_expression(exp_source);

	Expression* d = exp->derivative("x");
	ofstream output("output.txt");
	d->print(output);
	output.close();

	return EXIT_SUCCESS;
}
