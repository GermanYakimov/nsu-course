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

	virtual ~Expression() {};

};
