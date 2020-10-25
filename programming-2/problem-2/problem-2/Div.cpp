#include "expressions.h"

double Div::eval(const string denotion) const
{
	return left->eval(denotion) / right->eval(denotion);
}

Expression* Div::copy() const
{
	return new Div(left->copy(), right->copy());
}

Expression* Div::derivative(const string var) const
{
	return new Div(new Sub(new Mul(left->derivative(var), right->copy()), new Mul(left->copy(), right->derivative(var))), new Mul(right->copy(), right->copy()));
}

Expression* Div::reduce() const
{
	Expression* numerator_reduced = this->left->reduce();
	Expression* denominator_reduced = this->right->reduce();

	Number* denominator_reduced_number = dynamic_cast<Number*>(denominator_reduced);
	if (denominator_reduced_number && denominator_reduced_number->value == 1)
	{
		delete denominator_reduced;
		return numerator_reduced;
	}

	Number* numerator_reduced_number = dynamic_cast<Number*>(numerator_reduced);
	if (numerator_reduced_number && numerator_reduced_number->value == 0)
	{
		if (denominator_reduced_number && denominator_reduced_number->value != 0)
		{
			delete numerator_reduced;
			delete denominator_reduced;

			return new Number(0);
		}
		else
		{
			return new Div(numerator_reduced, denominator_reduced);
		}

	}

	if (numerator_reduced_number && denominator_reduced_number)
	{
		double value = numerator_reduced_number->value / denominator_reduced_number->value;
		delete numerator_reduced;
		delete denominator_reduced;

		return new Number(value);
	}

	return new Div(numerator_reduced, denominator_reduced);
}
