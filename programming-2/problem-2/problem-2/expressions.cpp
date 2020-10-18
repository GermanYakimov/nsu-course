#include <iostream>
#include <string>

using namespace std;

#include "expressions.h"

Number::Number(): value(0.) {}

Number::Number(const int val) : value(val * 1.) {}

Number::Number(const double val) : value(val) {}

double Number::eval(const string denotion) const
{
	return this->value;
}

void Number::print(ostream& out)  const
{
	out << value;
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

bool Number::operator==(const Expression* that) const
{
	Expression* that_reduced = that->reduce();

	Number* that_reduced_number = dynamic_cast<Number*>(that_reduced);

	return (that_reduced_number && (that_reduced_number->value == this->value));
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

void Variable::print(ostream& out) const
{
	out << name;
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
}

Expression* Variable::reduce() const
{
	return this->copy();
}

bool Variable::operator==(const Expression* that) const
{
	Expression* that_reduced = that->reduce();

	Variable* that_reduced_number = dynamic_cast<Variable*>(that_reduced);

	return (that_reduced_number && (that_reduced_number->name == this->name));
}


Add::Add(Expression* exp_1, Expression* exp_2) : term_1(exp_1), term_2(exp_2) {}

Add::Add() : term_1(nullptr), term_2(nullptr) {}

double Add::eval(const string denotion) const
{
	return term_1->eval(denotion) + term_2->eval(denotion);
}

void Add::print(ostream& out) const
{
	out << "(";
	term_1->print(out);
	out << "+";
	term_2->print(out);
	out << ")";
}

Expression* Add::copy() const
{
	return new Add(term_1->copy(), term_2->copy());
}

Expression* Add::derivative(const string var) const
{
	return new Add(term_1->derivative(var), term_2->derivative(var));
}

Expression* Add::reduce() const
{
	Expression* term_1_reduced = this->term_1->reduce();
	Expression* term_2_reduced = this->term_2->reduce();

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);
	if (term_1_reduced_number && term_1_reduced_number->eval("") == 0)
	{
		return term_2_reduced;
	}

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->eval("") == 0)
	{
		return term_1_reduced;
	}

	if (term_1_reduced_number && term_2_reduced_number)
	{
		return new Number(term_1_reduced_number->eval("") + term_2_reduced_number->eval(""));
	}

	return new Add(term_1_reduced, term_2_reduced);
}

bool Add::operator==(const Expression* that) const
{
	throw "not implemented";
}

Add::~Add()
{
	delete term_1;
	delete term_2;
}


Sub::Sub(Expression* exp_1, Expression* exp_2) : term_1(exp_1), term_2(exp_2) {}

Sub::Sub() : term_1(nullptr), term_2(nullptr) {}

double Sub::eval(const string denotion) const
{
	return term_1->eval(denotion) - term_2->eval(denotion);
}

void Sub::print(ostream& out) const
{
	out << "(";
	term_1->print(out);
	out << "-";
	term_2->print(out);
	out << ")";
}

Expression* Sub::copy() const
{
	return new Sub(term_1->copy(), term_2->copy());
}

Expression* Sub::derivative(const string var) const
{
	return new Sub(term_1->derivative(var), term_2->derivative(var));
}

Expression* Sub::reduce() const
{
	Expression* term_1_reduced = this->term_1->reduce();
	Expression* term_2_reduced = this->term_2->reduce();

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->eval("") == 0)
	{
		return term_1_reduced;
	}

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);

	if (term_1_reduced_number && term_2_reduced_number)
	{
		return new Number(term_1_reduced_number->eval("") - term_2_reduced_number->eval(""));
	}

	return new Sub(term_1_reduced, term_2_reduced);
}

bool Sub::operator==(const Expression* that) const
{
	throw "not implemented";
}

Sub::~Sub()
{
	delete term_1;
	delete term_2;
}


Mul::Mul(Expression* exp_1, Expression* exp_2) : factor_1(exp_1), factor_2(exp_2) {}

Mul::Mul() : factor_1(nullptr), factor_2(nullptr) {}

double Mul::eval(const string denotion) const
{
	return factor_1->eval(denotion) * factor_2->eval(denotion);
}

void Mul::print(ostream& out) const
{
	out << "(";
	factor_1->print(out);
	out << "*";
	factor_2->print(out);
	out << ")";
}

Expression* Mul::copy() const
{
	return new Mul(factor_1->copy(), factor_2->copy());
}

Expression* Mul::derivative(const string var) const
{
	return new Add(new Mul(factor_1->derivative(var), factor_2), new Mul(factor_1, factor_2->derivative(var)));
}

Expression* Mul::reduce() const
{
	Expression* factor_1_reduced = this->factor_1->reduce();
	Expression* factor_2_reduced = this->factor_2->reduce();

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

bool Mul::operator==(const Expression* that) const
{
	throw "not implemented";
}

Mul::~Mul()
{
	delete factor_1;
	delete factor_2;
}


Div::Div(Expression* num, Expression* den) : numerator(num), denominator(den) {}

Div::Div() : numerator(nullptr), denominator(nullptr) {}

double Div::eval(const string denotion) const
{
	return numerator->eval(denotion) / denominator->eval(denotion);
}

void Div::print(ostream& out) const
{
	out << "(";
	numerator->print(out);
	out << "/";
	denominator->print(out);
	out << ")";
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
	if (denominator_reduced_number && denominator_reduced_number->eval("") == 1)
	{
		return numerator_reduced;
	}

	Number* numerator_reduced_number = dynamic_cast<Number*>(numerator_reduced);
	if (numerator_reduced_number && numerator_reduced_number->eval("") == 0)
	{
		if (denominator_reduced_number && denominator_reduced_number->eval("") != 0)
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
		return new Number(numerator_reduced_number->eval("") / denominator_reduced_number->eval(""));
	}

	return new Div(numerator_reduced, denominator_reduced);
}

bool Div::operator==(const Expression* that) const
{
	throw "not implemented";
}

Div::~Div()
{
	delete numerator;
	delete denominator;
}
