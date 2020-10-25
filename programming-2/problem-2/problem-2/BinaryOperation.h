#pragma once
#include "expressions.h"


class BinaryOperation : public Expression
{
protected:
	Expression* left;
	Expression* right;
	string operation;

	BinaryOperation(string operation_symbol, Expression* exp_1, Expression* exp_2): operation(operation_symbol), left(exp_1), right(exp_2) {}

public:

	BinaryOperation() : operation(""), left(nullptr), right(nullptr) {}

	BinaryOperation(string operation_symbol) : operation(operation_symbol), left(nullptr), right(nullptr) {}

	virtual double eval(const string denotion) const = 0;

	virtual string to_string() const;

	virtual Expression* copy() const = 0;

	virtual Expression* derivative(const string var) const = 0;

	virtual Expression* reduce() const = 0;

	virtual ~BinaryOperation();

};
