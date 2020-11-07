#include <string>

#include "expressions.h"


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