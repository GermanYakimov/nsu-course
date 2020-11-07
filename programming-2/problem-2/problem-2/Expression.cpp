#include <iostream>
#include "expressions.h"


ostream& Expression::print(ostream& out) const
{
	out << this->to_string();
	return out;
}
