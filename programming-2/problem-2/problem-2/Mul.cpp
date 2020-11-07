#include "expressions.h"


double Mul::eval(const string denotion) const
{
	return left->eval(denotion) * right->eval(denotion);
}

Expression* Mul::copy() const
{
	return new Mul(left->copy(), right->copy());
}

Expression* Mul::derivative(const string var) const
{
	return new Add(new Mul(left->derivative(var), right->copy()), new Mul(left->copy(), right->derivative(var)));
}

Expression* Mul::reduce() const
{
	Expression* factor_1_reduced = this->left->reduce();
	Expression* factor_2_reduced = this->right->reduce();

	Number* factor_1_reduced_number = dynamic_cast<Number*>(factor_1_reduced);
	if (factor_1_reduced_number)
	{
		if (factor_1_reduced_number->value == 0)
		{
			delete factor_1_reduced;
			delete factor_2_reduced;

			return new Number(0);
		}
		else if (factor_1_reduced_number->value == 1)
		{
			delete factor_1_reduced;
			return factor_2_reduced;
		}
	}

	Number* factor_2_reduced_number = dynamic_cast<Number*>(factor_2_reduced);
	if (factor_2_reduced_number)
	{
		if (factor_2_reduced_number->value == 0)
		{
			delete factor_1_reduced;
			delete factor_2_reduced;

			return new Number(0);
		}
		else if (factor_2_reduced_number->value == 1)
		{
			return factor_1_reduced;
		}
	}

	if (factor_1_reduced_number && factor_2_reduced_number)
	{
		double value = factor_1_reduced_number->value * factor_2_reduced_number->value;
		delete factor_1_reduced;
		delete factor_2_reduced;

		return new Number(value);
	}

	return new Mul(factor_1_reduced, factor_2_reduced);
}
