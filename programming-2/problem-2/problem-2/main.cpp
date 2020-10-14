#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class Expression
{

public:
	Expression() {}

	virtual void print(ostream& out) = 0;

	virtual double eval(string denotion) = 0;

	virtual Expression* derivative(string var) = 0;
};


class Number : public Expression
{
	double value;

public:
	Number(int val): value(val * 1.0) {}

	Number(double val) : value(val) {}

	virtual double eval(string denotion)
	{
		return this->value;
	}

	virtual void print(ostream& out)
	{
		out << value;
	}

	virtual Expression* derivative(string var)
	{
		return new Number(0);
	}
};


class Variable : public Expression
{
	string name;

public:
	Variable(string var) : name(var) {}

	virtual double eval(string denotion)
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

	virtual void print(ostream& out)
	{
		out << name;
	}

	virtual Expression* derivative(string var)
	{
		if (var == this->name)
		{
			return new Number(1);
		}
	}
};


class Add : public Expression
{
	Expression* term_1;
	Expression* term_2;

public:
	Add(Expression* exp_1, Expression* exp_2): term_1(exp_1), term_2(exp_2) {}

	virtual double eval(string denotion)
	{
		return term_1->eval(denotion) + term_2->eval(denotion);
	}

	virtual void print(ostream& out)
	{
		out << "(";
		term_1->print(out);
		out << "+";
		term_2->print(out);
		out << ")";
	}

	virtual Expression* derivative(string var)
	{
		return new Add(term_1->derivative(var), term_2->derivative(var));
	}
};


class Sub : public Expression
{
	Expression* term_1;
	Expression* term_2;

public:
	Sub(Expression* exp_1, Expression* exp_2) : term_1(exp_1), term_2(exp_2) {}

	virtual double eval(string denotion)
	{
		return term_1->eval(denotion) - term_2->eval(denotion);
	}

	virtual void print(ostream& out)
	{
		out << "(";
		term_1->print(out);
		out << "-";
		term_2->print(out);
		out << ")";
	}

	virtual Expression* derivative(string var)
	{
		return new Sub(term_1->derivative(var), term_2->derivative(var));
	}
};


class Mul : public Expression
{
	Expression* factor_1;
	Expression* factor_2;

public:
	Mul(Expression* exp_1, Expression* exp_2): factor_1(exp_1), factor_2(exp_2) {}

	virtual double eval(string denotion)
	{
		return factor_1->eval(denotion) * factor_2->eval(denotion);
	}

	virtual void print(ostream& out)
	{
		out << "(";
		factor_1->print(out);
		out << "*";
		factor_2->print(out);
		out << ")";
	}

	virtual Expression* derivative(string var)
	{
		return new Add(new Mul(factor_1->derivative(var), factor_2), new Mul(factor_1, factor_2->derivative(var)));
	}
};


class Div : public Expression
{
	Expression* numerator;
	Expression* denominator;

public:
	Div(Expression* num, Expression* den): numerator(num), denominator(den) {}

	virtual double eval(string denotion)
	{
		return numerator->eval(denotion) / denominator->eval(denotion);
	}

	virtual void print(ostream& out)
	{
		out << "(";
		numerator->print(out);
		out << "/";
		denominator->print(out);
		out << ")";
	}

	virtual Expression* derivative(string var)
	{
		return new Div(new Sub(new Mul(numerator->derivative(var), denominator), new Mul(numerator, denominator->derivative(var))), new Mul(denominator, denominator));
	}
};


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
