#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "expressions.h"
#include "expression_builder.h"

int main()
{
	ifstream input("input.txt");
	string exp_source;
	input >> exp_source;
	input.close();

	ExpressionBuilder builder;

	Expression* exp = builder.build_expression(exp_source);

	Expression* d = exp->derivative("x");
	ofstream output("output.txt");
	d->print(output);
	output.close();

	return EXIT_SUCCESS;
}
