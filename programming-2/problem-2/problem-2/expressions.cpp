#include <iostream>
#include <string>

using namespace std;

#include "expressions.h"


Number::Number(int val) : value(val * 1.0) {}

Number::Number(double val) : value(val) {}

double Number::eval(string denotion)
{
	return this->value;
}

void Number::print(ostream& out)
{
	out << value;
}

Expression* Number::copy()
{
	return new Number(this->value);
}

Expression* Number::derivative(string var)
{
	return new Number(0);
}



Variable::Variable(string var) : name(var) {}

double Variable::eval(string denotion)
{

	size_t start_index = denotion.find(name + " <- ") + name.length() + 4;
	size_t end_index;
	size_t delimiter_pos = denotion.find(';', start_index);

	if (delimiter_pos == string::npos)
	{
		end_index = denotion.length() - 1;
	}
	else
	{
		end_index = delimiter_pos - 1;
	}

	string value = denotion.substr(start_index, end_index - start_index + 1);
	double value_num = stod(value);

	return value_num;
}

void Variable::print(ostream& out)
{
	out << name;
}

Expression* Variable::copy()
{
	return new Variable(this->name);
}

Expression* Variable::derivative(string var)
{
	if (var == this->name)
	{
		return new Number(1);
	}
}


Add::Add(Expression* exp_1, Expression* exp_2) : term_1(exp_1), term_2(exp_2) {}

Add::Add() : term_1(nullptr), term_2(nullptr) {}

double Add::eval(string denotion)
{
	return term_1->eval(denotion) + term_2->eval(denotion);
}

void Add::print(ostream& out)
{
	out << "(";
	term_1->print(out);
	out << "+";
	term_2->print(out);
	out << ")";
}

Expression* Add::copy()
{
	return new Add(term_1->copy(), term_2->copy());
}

Expression* Add::derivative(string var)
{
	return new Add(term_1->derivative(var), term_2->derivative(var));
}

Add::~Add()
{
	delete term_1;
	delete term_2;
}


Sub::Sub(Expression* exp_1, Expression* exp_2) : term_1(exp_1), term_2(exp_2) {}

Sub::Sub() : term_1(nullptr), term_2(nullptr) {}

double Sub::eval(string denotion)
{
	return term_1->eval(denotion) - term_2->eval(denotion);
}

void Sub::print(ostream& out)
{
	out << "(";
	term_1->print(out);
	out << "-";
	term_2->print(out);
	out << ")";
}

Expression* Sub::copy()
{
	return new Sub(term_1->copy(), term_2->copy());
}

Expression* Sub::derivative(string var)
{
	return new Sub(term_1->derivative(var), term_2->derivative(var));
}

Sub::~Sub()
{
	delete term_1;
	delete term_2;
}


Mul::Mul(Expression* exp_1, Expression* exp_2) : factor_1(exp_1), factor_2(exp_2) {}

Mul::Mul() : factor_1(nullptr), factor_2(nullptr) {}

double Mul::eval(string denotion)
{
	return factor_1->eval(denotion) * factor_2->eval(denotion);
}

void Mul::print(ostream& out)
{
	out << "(";
	factor_1->print(out);
	out << "*";
	factor_2->print(out);
	out << ")";
}

Expression* Mul::copy()
{
	return new Mul(factor_1->copy(), factor_2->copy());
}

Expression* Mul::derivative(string var)
{
	return new Add(new Mul(factor_1->derivative(var), factor_2), new Mul(factor_1, factor_2->derivative(var)));
}

Mul::~Mul()
{
	delete factor_1;
	delete factor_2;
}


Div::Div(Expression* num, Expression* den) : numerator(num), denominator(den) {}

Div::Div() : numerator(nullptr), denominator(nullptr) {}

double Div::eval(string denotion)
{
	return numerator->eval(denotion) / denominator->eval(denotion);
}

void Div::print(ostream& out)
{
	out << "(";
	numerator->print(out);
	out << "/";
	denominator->print(out);
	out << ")";
}

Expression* Div::copy()
{
	return new Div(numerator->copy(), denominator->copy());
}

Expression* Div::derivative(string var)
{
	return new Div(new Sub(new Mul(numerator->derivative(var), denominator), new Mul(numerator, denominator->derivative(var))), new Mul(denominator, denominator));
}

Div::~Div()
{
	delete numerator;
	delete denominator;
}


Expression* build_expression(string source)
{
	if (source.find('(') == string::npos)
	{
		try
		{
			double value = stod(source);
			return new Number(value);
		}
		catch (invalid_argument)
		{
			return new Variable(source);
		}
	}

	source = source.substr(1, source.length() - 2);

	int counter = 0;
	size_t main_operation_position = 0;

	for (size_t i = 0; i < source.length(); i++)
	{
		if (source[i] == '(')
		{
			counter++;
		}
		if (source[i] == ')')
		{
			counter--;
		}

		if ((source[i] == '+' || source[i] == '-' || source[i] == '*' || source[i] == '/') && counter == 0)
		{
			main_operation_position = i;
			break;
		}
	}

	if (main_operation_position == 0)
	{
		throw "invalid expression";
	}

	string subexpression_1 = source.substr(0, main_operation_position);
	string subexpression_2 = source.substr(main_operation_position + 1, source.length() - main_operation_position - 1);

	if (source[main_operation_position] == '+')
	{
		return new Add(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (source[main_operation_position] == '-')
	{
		return new Sub(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (source[main_operation_position] == '*')
	{
		return new Mul(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (source[main_operation_position] == '/')
	{
		return new Div(build_expression(subexpression_1), build_expression(subexpression_2));
	}

	throw "invalid operation";
}
