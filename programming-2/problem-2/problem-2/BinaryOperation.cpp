#include "expressions.h"


string BinaryOperation::to_string() const
{
	return this->left->to_string() + this->operation + this->right->to_string();
}

BinaryOperation::~BinaryOperation()
{
	delete this->left;
	delete this->right;
}
