#pragma once
#include "expressions.h"


class Div : public BinaryOperation
{
	friend class ExpressionBuilder;

	Div(Expression* num, Expression* den) : BinaryOperation("/", num, den) {}

public:
	Div() : BinaryOperation("/") {}

	virtual double eval(const string denotion) const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

};