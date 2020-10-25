#include "expressions.h"


double Sub::eval(const string denotion) const
{
	return left->eval(denotion) - right->eval(denotion);
}

Expression* Sub::copy() const
{
	return new Sub(left->copy(), right->copy());
}

Expression* Sub::derivative(const string var) const
{
	return new Sub(left->derivative(var), right->derivative(var));
}

Expression* Sub::reduce() const
{
	Expression* term_1_reduced = this->left->reduce();
	Expression* term_2_reduced = this->right->reduce();

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->value == 0)
	{
		return term_1_reduced;
	}

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);

	if (term_1_reduced_number && term_2_reduced_number)
	{
		double value = term_1_reduced_number->value - term_2_reduced_number->value;
		delete term_1_reduced;
		delete term_2_reduced;

		return new Number(value);
	}

	if (term_1_reduced->to_string() == term_2_reduced->to_string())
	{
		delete term_1_reduced;
		delete term_2_reduced;

		return new Number(0);
	}

	return new Sub(term_1_reduced, term_2_reduced);
}
