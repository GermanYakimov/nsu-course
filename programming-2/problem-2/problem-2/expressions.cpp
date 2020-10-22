#include <iostream>
#include <string>

using namespace std;

#include "expressions.h"

ostream& Expression::print(ostream& out) const
{
	out << this->to_string();
	return out;
}


Number::Number(): value(0.) {}

Number::Number(const int val) : value(val * 1.) {}

Number::Number(const double val) : value(val) {}

double Number::eval(const string denotion) const
{
	return this->value;
}

string Number::to_string() const
{
	return std::to_string(this->value);
}

Expression* Number::copy()  const
{
	return new Number(this->value);
}

Expression* Number::derivative(const string var) const
{
	return new Number(0);
}

Expression* Number::reduce()  const
{
	return this->copy();
}


Variable::Variable(const string var) : name(var) {}

double Variable::eval(const string denotion) const
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

string Variable::to_string() const
{
	return this->name;
}

Expression* Variable::copy() const
{
	return new Variable(this->name);
}

Expression* Variable::derivative(const string var) const
{
	if (var == this->name)
	{
		return new Number(1);
	}
	return new Number(0);
}

Expression* Variable::reduce() const
{
	return this->copy();
}


Add::Add(Expression* exp_1, Expression* exp_2) : left(exp_1), right(exp_2) {}

Add::Add() : left(nullptr), right(nullptr) {}

double Add::eval(const string denotion) const
{
	return left->eval(denotion) + right->eval(denotion);
}

string Add::to_string() const
{
	return this->left->to_string() + "+" + this->right->to_string();
}

Expression* Add::copy() const
{
	return new Add(left->copy(), right->copy());
}

Expression* Add::derivative(const string var) const
{
	return new Add(left->derivative(var), right->derivative(var));
}

Expression* Add::reduce() const
{
	Expression* term_1_reduced = this->left->reduce();
	Expression* term_2_reduced = this->right->reduce();

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);
	if (term_1_reduced_number && term_1_reduced_number->value == 0)
	{
		return term_2_reduced;
	}

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->value == 0)
	{
		return term_1_reduced;
	}

	if (term_1_reduced_number && term_2_reduced_number)
	{
		return new Number(term_1_reduced_number->value + term_2_reduced_number->value);
	}

	return new Add(term_1_reduced, term_2_reduced);
}

Add::~Add()
{
	delete left;
	delete right;
}


Sub::Sub(Expression* exp_1, Expression* exp_2) : left(exp_1), right(exp_2) {}

Sub::Sub() : left(nullptr), right(nullptr) {}

double Sub::eval(const string denotion) const
{
	return left->eval(denotion) - right->eval(denotion);
}

string Sub::to_string() const
{
	return this->left->to_string() + "-" + this->right->to_string();
}

Expression* Sub::copy() const
{
	return new Sub(left->copy(), right->copy());
}

Expression* Sub::derivative(const string var) const
{
	return new Sub(left->derivative(var), right->derivative(var));
}

Expression* Sub::reduce() const
{
	Expression* term_1_reduced = this->left->reduce();
	Expression* term_2_reduced = this->right->reduce();

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->value == 0)
	{
		return term_1_reduced;
	}

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);

	if (term_1_reduced_number && term_2_reduced_number)
	{
		return new Number(term_1_reduced_number->value - term_2_reduced_number->value);
	}

	if (term_1_reduced->to_string() == term_2_reduced->to_string())
	{
		return new Number(0);
	}

	return new Sub(term_1_reduced, term_2_reduced);
}

Sub::~Sub()
{
	delete left;
	delete right;
}


Mul::Mul(Expression* exp_1, Expression* exp_2) : left(exp_1), right(exp_2) {}

Mul::Mul() : left(nullptr), right(nullptr) {}

double Mul::eval(const string denotion) const
{
	return left->eval(denotion) * right->eval(denotion);
}

string Mul::to_string() const
{
	return this->left->to_string() + "*" + this->right->to_string();
}

Expression* Mul::copy() const
{
	return new Mul(left->copy(), right->copy());
}

Expression* Mul::derivative(const string var) const
{
	return new Add(new Mul(left->derivative(var), right), new Mul(left, right->derivative(var)));
}

Expression* Mul::reduce() const
{
	Expression* factor_1_reduced = this->left->reduce();
	Expression* factor_2_reduced = this->right->reduce();

	Number* factor_1_reduced_number = dynamic_cast<Number*>(factor_1_reduced);
	if (factor_1_reduced_number)
	{
		if (factor_1_reduced_number->eval("") == 0)
		{
			return new Number(0);
		}
		else if (factor_1_reduced_number->eval("") == 1)
		{
			return factor_2_reduced;
		}
	}

	Number* factor_2_reduced_number = dynamic_cast<Number*>(factor_2_reduced);
	if (factor_2_reduced_number)
	{
		if (factor_2_reduced_number->eval("") == 0)
		{
			return new Number(0);
		}
		else if (factor_2_reduced_number->eval("") == 1)
		{
			return factor_1_reduced;
		}
	}

	if (factor_1_reduced_number && factor_2_reduced_number)
	{
		return new Number(factor_1_reduced_number->eval("") * factor_2_reduced_number->eval(""));
	}

	return new Mul(factor_1_reduced, factor_2_reduced);
}

Mul::~Mul()
{
	delete left;
	delete right;
}


Div::Div(Expression* num, Expression* den) : numerator(num), denominator(den) {}

Div::Div() : numerator(nullptr), denominator(nullptr) {}

double Div::eval(const string denotion) const
{
	return numerator->eval(denotion) / denominator->eval(denotion);
}

string Div::to_string() const
{
	return this->numerator->to_string() + "/" + this->denominator->to_string();
}

Expression* Div::copy() const
{
	return new Div(numerator->copy(), denominator->copy());
}

Expression* Div::derivative(const string var) const
{
	return new Div(new Sub(new Mul(numerator->derivative(var), denominator), new Mul(numerator, denominator->derivative(var))), new Mul(denominator, denominator));
}

Expression* Div::reduce() const
{
	Expression* numerator_reduced = this->numerator->reduce();
	Expression* denominator_reduced = this->denominator->reduce();

	Number* denominator_reduced_number = dynamic_cast<Number*>(denominator_reduced);
	if (denominator_reduced_number && denominator_reduced_number->value == 1)
	{
		return numerator_reduced;
	}

	Number* numerator_reduced_number = dynamic_cast<Number*>(numerator_reduced);
	if (numerator_reduced_number && numerator_reduced_number->value == 0)
	{
		if (denominator_reduced_number && denominator_reduced_number->value != 0)
		{
			return new Number(0);
		}
		else
		{
			// division by zero can be here
			return new Div(numerator_reduced, denominator_reduced);
		}

	}

	if (numerator_reduced_number && denominator_reduced_number)
	{
		return new Number(numerator_reduced_number->value / denominator_reduced_number->value);
	}

	return new Div(numerator_reduced, denominator_reduced);
}

Div::~Div()
{
	delete numerator;
	delete denominator;
}
