#pragma once
#include "expressions.h"


class Sub : public BinaryOperation
{
	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Sub(Expression* exp_1, Expression* exp_2): BinaryOperation("-", exp_1, exp_2) {}

public:

	Sub(): BinaryOperation("-") {}

	virtual double eval(const string denotion) const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

};
