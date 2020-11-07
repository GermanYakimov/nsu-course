#pragma once
#include "expressions.h"


class Mul : public BinaryOperation
{
	friend class Div;
	friend class ExpressionBuilder;

	Mul(Expression* exp_1, Expression* exp_2): BinaryOperation("*", exp_1, exp_2) {}

public:

	Mul(): BinaryOperation("*") {}

	virtual double eval(const string denotion) const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

};
