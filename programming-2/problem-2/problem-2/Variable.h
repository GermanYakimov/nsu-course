#pragma once
#include "expressions.h"


class Variable : public Expression
{
	string name;

public:

	Variable(): name("x") {}

	Variable(const string var): name(var) {}

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual ~Variable() {};

};