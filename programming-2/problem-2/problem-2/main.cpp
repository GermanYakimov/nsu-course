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

	Expression* d_re = d->reduce();

	d_re->print(cout);
	cout << endl;
	Expression* d_d = d_re->derivative("x");
	Expression* d_d_re = d_d->reduce();
	d_d_re->print(cout);

	delete exp;
	delete d;
	delete d_re;
	delete d_d_re;
	delete d_d;

	return EXIT_SUCCESS;
}
