#pragma once
#include <fstream>

using namespace std;

class Expression
{
public:
	Expression() {}

	virtual void print(ostream& out) = 0;

	virtual double eval(string denotion) = 0;

	virtual Expression* derivative(string var) = 0;

	virtual Expression* copy() = 0;

	virtual Expression* reduce() = 0;

	virtual bool operator==(Expression* that) = 0;
};


class Number : public Expression
{
	double value;

public:
	Number(int val);

	Number(double val);

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

};


class Variable : public Expression
{
	string name;

public:

	Variable(string var);

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

};


class Add : public Expression
{
	Expression* term_1;
	Expression* term_2;

	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Add(Expression* exp_1, Expression* exp_2);

public:

	Add();

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

	~Add();

};


class Sub : public Expression
{
	Expression* term_1;
	Expression* term_2;

	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Sub(Expression* exp_1, Expression* exp_2);

public:

	Sub();

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

	~Sub();
};


class Mul : public Expression
{
	Expression* factor_1;
	Expression* factor_2;

	friend class Div;
	friend class ExpressionBuilder;

	Mul(Expression* exp_1, Expression* exp_2);

public:

	Mul();

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

	~Mul();
};


class Div : public Expression
{
	Expression* numerator;
	Expression* denominator;

	friend class ExpressionBuilder;
	
	Div(Expression* num, Expression* den);

public:
	Div();

	virtual double eval(string denotion);

	virtual void print(ostream& out);

	virtual Expression* copy();

	virtual Expression* derivative(string var);

	virtual Expression* reduce();

	virtual bool operator==(Expression* that);

	~Div();
};
