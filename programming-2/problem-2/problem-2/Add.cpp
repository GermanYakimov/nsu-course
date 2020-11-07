#include "expressions.h"


double Add::eval(const string denotion) const
{
	return left->eval(denotion) + right->eval(denotion);
}

Expression* Add::copy() const
{
	return new Add(left->copy(), right->copy());
}

Expression* Add::derivative(const string var) const
{
	return new Add(left->derivative(var), right->derivative(var));
}

Expression* Add::reduce() const
{
	Expression* term_1_reduced = this->left->reduce();
	Expression* term_2_reduced = this->right->reduce();

	Number* term_1_reduced_number = dynamic_cast<Number*>(term_1_reduced);
	if (term_1_reduced_number && term_1_reduced_number->value == 0)
	{
		return term_2_reduced;
	}

	Number* term_2_reduced_number = dynamic_cast<Number*>(term_2_reduced);
	if (term_2_reduced_number && term_2_reduced_number->value == 0)
	{
		return term_1_reduced;
	}

	if (term_1_reduced_number && term_2_reduced_number)
	{
		return new Number(term_1_reduced_number->value + term_2_reduced_number->value);
	}

	return new Add(term_1_reduced, term_2_reduced);
}
