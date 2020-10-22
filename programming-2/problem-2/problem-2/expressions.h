#pragma once

#include <fstream>

using namespace std;

class Expression
{
public:
	Expression() {}

	virtual ostream& print(ostream& out) const;

	virtual double eval(const string denotion) const = 0;

	virtual string to_string() const = 0;

	virtual Expression* derivative(const string var) const = 0;

	virtual Expression* copy() const = 0;

	virtual Expression* reduce() const = 0;

	virtual bool operator==(const Expression* that) const = 0;

};


class Number : public Expression
{
	double value;

	friend class Add;
	friend class Sub;
	friend class Mul;
	friend class Div;

public:
	Number();

	Number(const int val);

	Number(const double val);

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

};


class Variable : public Expression
{
	string name;

public:

	Variable(const string var);

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

};


class Add : public Expression
{
	Expression* left;
	Expression* right;

	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Add(Expression* exp_1, Expression* exp_2);

public:

	Add();

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

	~Add();

};


class Sub : public Expression
{
	Expression* left;
	Expression* right;

	friend class Mul;
	friend class Div;
	friend class ExpressionBuilder;

	Sub(Expression* exp_1, Expression* exp_2);

public:

	Sub();

	virtual double eval(const string denotion) const;
	
	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

	~Sub();
};


class Mul : public Expression
{
	Expression* left;
	Expression* right;

	friend class Div;
	friend class ExpressionBuilder;

	Mul(Expression* exp_1, Expression* exp_2);

public:

	Mul();

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

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

	virtual double eval(const string denotion) const;

	virtual string to_string() const;

	virtual Expression* copy() const;

	virtual Expression* derivative(const string var) const;

	virtual Expression* reduce() const;

	virtual bool operator==(const Expression* that) const;

	~Div();
};
