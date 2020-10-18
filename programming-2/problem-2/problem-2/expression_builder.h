#pragma once

#include<vector>
#include<iostream>
#include<string>
using namespace std;

#include "expressions.h"


class ExpressionBuilder
{
	short priority(const char token);

	vector<string> split_into_tokens(string expression);

	vector<string> build_reverse_polish_notation(vector<string> tokens);

	string arrange_brackets(string expression);

	Expression* build(string expression);

	string remove_spaces(string expression);

public:
	Expression* build_expression(string expression);
};
