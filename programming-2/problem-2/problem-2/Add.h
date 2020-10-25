#pragma once
#include "expressions.h"


class Add : public BinaryOperation
{
	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Add(Expression* exp_1, Expression* exp_2): BinaryOperation("+", exp_1, exp_2) {}

public:
	Add() : BinaryOperation("+") {}

	virtual double eval(const string denotion) const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

};
