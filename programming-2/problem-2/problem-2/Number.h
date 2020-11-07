#pragma once
#include "expressions.h"


class Number : public Expression
{
	double value;

	friend class Add;
	friend class Sub;
	friend class Mul;
	friend class Div;

public:
	Number(): value(0.) {}

	Number(const int val): value(val * 1.0) {}

	Number(const double val): value(val) {}

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual ~Number() {};

};