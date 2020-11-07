#include <string>

#include "expressions.h"


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
