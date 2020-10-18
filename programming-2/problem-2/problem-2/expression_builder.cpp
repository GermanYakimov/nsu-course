#include <stack>

#include "expression_builder.h"
#include "expressions.h"

short ExpressionBuilder::priority(const char token)
{
    switch (token)
    {
    case '(':
        return 1;

    case ')':
        return 2;

    case '+':
    case '-':
        return 3;

    case '*':
    case '/':
        return 4;

    default:
        return 5;
    }
}

vector<string> ExpressionBuilder::split_into_tokens(string expression)
{
    string new_token;
    vector <string> tokens;

    for (size_t i = 0; i < expression.length(); i++)
    {
        new_token.clear();

        if (isalpha(expression[i]))
        {
            new_token += expression[i];
            size_t j = 1;

            while (isalpha(expression[i + j]))
            {
                new_token += expression[i + j];
                j++;
            }

            i += j - 1;
        }

        else if (isdigit(expression[i]))
        {
            size_t dots_counter = 0;

            new_token += expression[i];
            size_t j = 1;

            while (isdigit(expression[i + j]) || expression[i + j] == '.')
            {
                if (expression[i + j] == '.')
                {
                    if (dots_counter == 1)
                    {
                        throw "invalid token";
                    }
                    dots_counter++;
                }
                new_token += expression[i + j];
                j++;
            }

            i += j - 1;
        }

        else if (this->priority(expression[i]) != 5)
        {
            new_token = string(1, expression[i]);
        }

        tokens.push_back(new_token);
    }

    return tokens;
}

vector<string> ExpressionBuilder::build_reverse_polish_notation(vector<string> tokens)
{
    stack <string> stack_of_tokens;
    vector <string> rpn_tokens;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "(")
        {
            stack_of_tokens.push(tokens[i]);
            continue;
        }

        if (isalpha(tokens[i][0]))
        {
            rpn_tokens.push_back(tokens[i]);
            continue;
        }

        if (tokens[i] == ")")
        {
            while (stack_of_tokens.top() != "(")
            {
                rpn_tokens.push_back(stack_of_tokens.top());
                stack_of_tokens.pop();
            }

            stack_of_tokens.pop();
            continue;
        }

        if (stack_of_tokens.empty())
        {
            stack_of_tokens.push(tokens[i]);
        }
        else
        {
            while (!stack_of_tokens.empty() && (priority(stack_of_tokens.top()[0]) >= priority(tokens[i][0])))
            {
                rpn_tokens.push_back(stack_of_tokens.top());
                stack_of_tokens.pop();
            }

            stack_of_tokens.push(tokens[i]);
        }
    }

    while (!stack_of_tokens.empty())
    {
        rpn_tokens.push_back(stack_of_tokens.top());
        stack_of_tokens.pop();
    }

    return rpn_tokens;
}

string ExpressionBuilder::arrange_brackets(string expression)
{
    vector<string> tokens = this->split_into_tokens(expression);
    vector<string> rpn_tokens = this->build_reverse_polish_notation(tokens);

    size_t i = 0;

    while (rpn_tokens.size() != 1)
    {
        if (this->priority(rpn_tokens[i][0]) != 3 && this->priority(rpn_tokens[i][0]) != 4)
        {
            i++;
            continue;
        }

        rpn_tokens[i] = "(" + rpn_tokens[i - 2] + rpn_tokens[i] + rpn_tokens[i - 1] + ")";

        rpn_tokens.erase(rpn_tokens.begin() + i - 1);
        rpn_tokens.erase(rpn_tokens.begin() + i - 2);

        i = 0;
    }

    return rpn_tokens[0];
}

Expression* ExpressionBuilder::build(string expression)
{
    if (expression.find('(') == string::npos)
	{
		try
		{
			double value = stod(expression);
			return new Number(value);
		}
		catch (invalid_argument)
		{
			return new Variable(expression);
		}
	}

    expression = expression.substr(1, expression.length() - 2);

	int counter = 0;
	size_t main_operation_position = 0;

	for (size_t i = 0; i < expression.length(); i++)
	{
		if (expression[i] == '(')
		{
			counter++;
		}
		if (expression[i] == ')')
		{
			counter--;
		}

		if ((expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') && counter == 0)
		{
			main_operation_position = i;
			break;
		}
	}

	if (main_operation_position == 0)
	{
		throw "invalid expression";
	}

	string subexpression_1 = expression.substr(0, main_operation_position);
	string subexpression_2 = expression.substr(main_operation_position + 1, expression.length() - main_operation_position - 1);

	if (expression[main_operation_position] == '+')
	{
		return new Add(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (expression[main_operation_position] == '-')
	{
		return new Sub(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (expression[main_operation_position] == '*')
	{
		return new Mul(build_expression(subexpression_1), build_expression(subexpression_2));
	}
	if (expression[main_operation_position] == '/')
	{
		return new Div(build_expression(subexpression_1), build_expression(subexpression_2));
	}

	throw "invalid operation";
}

string ExpressionBuilder::remove_spaces(string expression)
{
    string result;

    for (size_t i = 0; i < expression.length(); i++)
    {
        if (!isspace(expression[i]))
        {
            result += expression[i];
        }
    }

    return result;
}


Expression* ExpressionBuilder::build_expression(string expression)
{
    expression = this->arrange_brackets(this->remove_spaces(expression));

    return this->build(expression);
}
