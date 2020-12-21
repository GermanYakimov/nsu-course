#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <fstream>

#include "DFA.h"

using namespace std;

constexpr auto EMPTY_SET = "empty";


vector<string> split(string source, char delimiter)
{
	if (source.find(delimiter) == string::npos)
	{
		return { source };
	}

	size_t current = 0, next;
	vector<string> tokens;

	while (source.find(delimiter, current) != string::npos)
	{
		next = source.find(delimiter, current);
		tokens.push_back(source.substr(current, next - current));
		current = next + 1;
	}
	tokens.push_back(source.substr(current, source.length() - current));

	return tokens;
}

string join(vector<string> sources, char delimiter)
{
	if (sources.empty())
	{
		return string();
	}

	string result;

	for (string str : sources)
	{
		result += str + delimiter;
	}

	return result.substr(0, result.length() - 1);
}

DFA::DFA(const DFA& that)
{
	this->states = that.states;
	this->transition_functions = that.transition_functions;
	this->initial_state = that.initial_state;
	this->alphabet = that.alphabet;
}

DFA DFA::build_from_stream(istream& input)
{
	size_t n, f, tmp, p;
	string from, to;
	char symbol;

	DFA::NFA nfa;

	input >> n;
	input >> nfa.initial_state;

	for (size_t i = 0; i < n; i++)
	{
		nfa.states[to_string(i)] = false;
	}

	input >> f;

	for (size_t _ = 0; _ < f; _++)
	{
		input >> tmp;

		nfa.states[to_string(tmp)] = true;
	}

	input >> p;

	for (size_t _ = 0; _ < p; _++)
	{
		input >> from >> to >> symbol;

		if (symbol == '#')  // # means empty symbol
		{
			symbol = ' ';
		}
		nfa.transition_functions.insert({ symbol, { from, to } });

		if (symbol != ' ')
		{
			nfa.alphabet.insert(symbol);
		}
	}

	return DFA::determine(nfa);
}

unordered_set<string> DFA::possible_states_next_step(char c, vector<string> states_list, DFA::NFA nfa)
{
	unordered_set<string> result;

	auto equal_range = nfa.transition_functions.equal_range(c);

	for (string state : states_list)
	{
		for (auto it = equal_range.first; it != equal_range.second; it++) {
			if (it->second.first == state)
			{
				result.insert(it->second.second);
			}
		}
	}

	return result;
}

void DFA::rename_state(string from, string to)
{
	if (this->states.find(from) == this->states.end())
	{
		throw invalid_argument("Can't find state named " + from);
	}
	if (this->states.find(to) != this->states.end())
	{
		throw invalid_argument("State named " + to + " already exists.");
	}

	for (char symbol : this->alphabet)
	{
		string tmp = this->transition_functions.find({ symbol, from })->second;
		this->transition_functions.erase({ symbol, from });
		this->transition_functions.insert({ {symbol, to}, tmp });
	}

	for (auto& function : this->transition_functions)
	{
		if (function.second == from)
		{
			function.second = to;
		}
	}

	this->states.insert({ to, this->states.find(from)->second });
	this->states.erase(from);

	if (this->initial_state == from)
	{
		this->initial_state = to;
	}
}

bool DFA::is_e_nfa(DFA::NFA e_nfa)
{
	for (auto function : e_nfa.transition_functions)
	{
		if (function.first == ' ')
		{
			return true;
		}
	}

	return false;
}

unordered_set<string> DFA::possible_states_e(string start, unordered_set<string> buffer, DFA::NFA e_nfa)
{
	unordered_set<string> next_states = DFA::possible_states_next_step(' ', { start }, e_nfa);

	for (auto state : next_states)
	{
		if (buffer.find(state) == buffer.end())
		{
			buffer.insert(state);
			buffer = DFA::possible_states_e(state, buffer, e_nfa);
		}
	}

	return buffer;
}

DFA::NFA DFA::e_nfa_to_nfa(DFA::NFA e_nfa)
{
	if (!DFA::is_e_nfa(e_nfa))
	{
		return e_nfa;
	}

	DFA::NFA result;
	unordered_set<string> initial_state_set = { e_nfa.initial_state };

	initial_state_set = DFA::possible_states_e(e_nfa.initial_state, initial_state_set, e_nfa);
	vector<string> initial_state_vec(initial_state_set.begin(), initial_state_set.end());
	sort(initial_state_vec.begin(), initial_state_vec.end());
	string initial_state_str = join(initial_state_vec, '#');

	result.initial_state = initial_state_str;
	result.states.insert({ initial_state_str, e_nfa.states.find(e_nfa.initial_state)->second });

	queue<string> buffer;
	unordered_set<string> done_states;
	buffer.push(initial_state_str);

	while (!buffer.empty())
	{
		string current_state = buffer.front();
		buffer.pop();
		vector<string> states = split(current_state, '#');

		for (auto c : e_nfa.alphabet)
		{
			unordered_set<string> all_possible_states;
			vector<string> states_vec(states.begin(), states.end());
			unordered_set<string> next_states = DFA::possible_states_next_step(c, states_vec, e_nfa);

			if (next_states.empty())
			{
				continue;
			}

			for (auto state : next_states)
			{
				unordered_set<string> states_e = { state };
				states_e = DFA::possible_states_e(state, states_e, e_nfa);

				for (auto tmp_state : states_e)
				{
					all_possible_states.insert(tmp_state);
				}
			}
			vector<string> all_possible_states_vec(all_possible_states.begin(), all_possible_states.end());
			sort(all_possible_states_vec.begin(), all_possible_states_vec.end());

			bool is_finite = false;
			for (string str : all_possible_states_vec)
			{
				if (e_nfa.states.find(str)->second)
				{
					is_finite = true;
					break;
				}
			}

			string states_result_str = join(all_possible_states_vec, '#');
			result.transition_functions.insert({ c, { current_state, states_result_str } });

			if (done_states.find(states_result_str) == done_states.end())
			{
				buffer.push(states_result_str);
				result.states.insert({ states_result_str, is_finite });
			}
		}
		done_states.insert(current_state);
	}

	result.alphabet = e_nfa.alphabet;

	return result;
}

DFA DFA::determine(DFA::NFA e_nfa)
{
	DFA::NFA nfa = DFA::e_nfa_to_nfa(e_nfa);
	return DFA::nfa_to_dfa(nfa);
}

DFA DFA::nfa_to_dfa(DFA::NFA nfa)
{
	DFA result;
	result.initial_state = nfa.initial_state;
	result.alphabet = nfa.alphabet;

	queue<string> Q;

	Q.push(nfa.initial_state);
	result.states.insert({ nfa.initial_state, nfa.states.find(nfa.initial_state)->second });

	while (!Q.empty())
	{
		string top = Q.front();
		Q.pop();

		vector<string> states_list = split(top, '_');

		for (char c : nfa.alphabet)
		{
			bool is_finite = false;

			unordered_set<string> states_result = DFA::possible_states_next_step(c, states_list, nfa);

			if (states_result.empty())
			{
				if (result.states.find(EMPTY_SET) == result.states.end())
				{
					result.states.insert({ EMPTY_SET, false });

					for (char c_ : nfa.alphabet)
					{
						result.transition_functions.insert({ { c_, EMPTY_SET }, EMPTY_SET });
					}
				}

				result.transition_functions.insert({ { c, top }, EMPTY_SET });
				continue;
			}

			vector<string> states_result_vec(states_result.begin(), states_result.end());
			sort(states_result_vec.begin(), states_result_vec.end());

			for (string str : states_result_vec)
			{
				if (nfa.states.find(str)->second)
				{
					is_finite = true;
					break;
				}
			}

			string states_result_str = join(states_result_vec, '_');

			if (result.states.find(states_result_str) == result.states.end())
			{
				result.states.insert({ states_result_str, is_finite });
				Q.push(states_result_str);
			}

			result.transition_functions.insert({ {c, top}, states_result_str });
		}
	}

	result = DFA::enumerate_states(result, 0);

	return result;
}

void DFA::print()
{
	for (auto state : this->states)
	{
		cout << state.first << " " << state.second << endl;
	}
	cout << endl;

	for (auto func : this->transition_functions)
	{
		cout << func.first.second << " " << func.second << " " << func.first.first << endl;
	}
}

string DFA::step(string state, char c)
{
	auto result = this->transition_functions.find({ c, state });
	if (result == this->transition_functions.end())
	{
		return EMPTY_SET;
	}

	return result->second;
}

DFA DFA::enumerate_states(DFA a, size_t start)
{
	auto states = a.states;
	for (auto state : states)
	{
		a.rename_state(state.first, "!" + state.first);
	}

	size_t state_index = start;

	states = a.states;
	for (auto state : states)
	{
		a.rename_state(state.first, to_string(state_index));
		state_index++;
	}

	return a;
}

string DFA::recognize(string source)
{
	string current_state = this->initial_state;

	for (char c : source)
	{
		current_state = this->step(current_state, c);
	}

	if (this->states.find(current_state)->second)
	{
		return "YES";
	}

	return "NO";
}

DFA DFA::kleene_star(DFA a)
{
	DFA::NFA result;
	result.states = a.states;
	result.alphabet = a.alphabet;
	result.initial_state = a.initial_state;

	for (auto state : a.states)
	{
		if (state.second)
		{
			result.transition_functions.insert({ ' ', { state.first, a.initial_state } });
		}
	}

	for (auto function : a.transition_functions)
	{
		result.transition_functions.insert({ function.first.first, {function.first.second, function.second} });
	}

	result.states.erase(a.initial_state);
	result.states.insert({ a.initial_state, true });

	return DFA::determine(result);
}

DFA::NFA DFA::to_nfa()
{
	DFA::NFA result;
	result.alphabet.insert(this->alphabet.begin(), this->alphabet.end());
	result.states.insert(this->states.begin(), this->states.end());

	for (auto function : this->transition_functions)
	{
		result.transition_functions.insert({ function.first.first, {function.first.second, function.second} });
	}

	return result;
}

DFA DFA::build_fa(string regexp)
{
	if (regexp.find('(') == string::npos && regexp.find('|') == string::npos && regexp.find('*') == string::npos)
	{
		DFA::NFA result;
		result.initial_state = "0";

		size_t size = regexp.length();
		for (size_t i = 0; i < size; i++)
		{
			result.states.insert({ to_string(i), false });
		}
		result.states.insert({ to_string(size), true });

		for (size_t i = 0; i < size; i++)
		{
			result.transition_functions.insert({ regexp[i], {to_string(i), to_string(i + 1)} });
			result.alphabet.insert(regexp[i]);
		}

		return DFA::determine(result);
	}

	if (regexp.back() == '*')
	{
		regexp = regexp.substr(0, regexp.length() - 1);
		regexp = regexp.substr(1, regexp.length() - 2);

		return DFA::kleene_star(DFA::build_fa(regexp));
	}

	{
		regexp = regexp.substr(1, regexp.length() - 2);
		if (regexp.back() == '*')
		{
			return DFA::build_fa(regexp);
		}

		int counter = 0;
		size_t index = 1;

		for (auto c : regexp)
		{
			if (c == '(')
			{
				counter++;
			}
			else if (c == ')')
			{
				counter--;
			}

			if (counter < 0)
			{
				throw invalid_argument("invalid regexp");
			}

			if (counter == 0)
			{
				break;
			}
			index++;
		}

		if (regexp[index] == '|')
		{
			string regexp_left = regexp.substr(1, index - 2);
			string regexp_right = regexp.substr(index + 2, regexp.length() - index - 3);

			DFA left = DFA::build_fa(regexp_left);
			DFA right = DFA::build_fa(regexp_right);

			return left | right;
		}
		else if (regexp[index] == '(')
		{
			string regexp_left = regexp.substr(1, index - 2);
			string regexp_right = regexp.substr(index + 1, regexp.length() - index - 2);

			DFA left = DFA::build_fa(regexp_left);
			DFA right = DFA::build_fa(regexp_right);

			return left & right;
		}

		throw invalid_argument("invalid regexp");
	}
}

DFA DFA::build_from_regexp(string regexp)
{
	return DFA::build_fa(regexp);
}

DFA operator&(DFA left, DFA right)
{
	left = DFA::enumerate_states(left, 0);
	right = DFA::enumerate_states(right, left.states.size());

	for (auto& state : left.states)
	{
		state.second = false;
	}

	DFA::NFA result = left.to_nfa() + right.to_nfa();
	result.initial_state = left.initial_state;

	unordered_set<string> left_final_states;
	for (auto state : left.states)
	{
		if (state.second)
		{
			left_final_states.insert(state.first);
		}
	}

	for (auto state : left_final_states)
	{
		result.transition_functions.insert({ ' ', { state, right.initial_state } });
	}

	return DFA::determine(result);
}

DFA operator|(DFA left, DFA right)
{
	left = DFA::enumerate_states(left, 1);
	right = DFA::enumerate_states(right, left.states.size() + 1);
	DFA::NFA result = left.to_nfa() + right.to_nfa();

	result.initial_state = "0";
	result.states.insert({ "0", false });

	result.transition_functions.insert({ ' ', {"0", left.initial_state} });
	result.transition_functions.insert({ ' ', {"0", right.initial_state} });

	return DFA::determine(result);
}

DFA::NFA operator+(DFA::NFA left, DFA::NFA right)
{
	DFA::NFA result;
	result.alphabet.insert(left.alphabet.begin(), left.alphabet.end());
	result.alphabet.insert(right.alphabet.begin(), right.alphabet.end());

	result.states.insert(left.states.begin(), left.states.end());
	result.states.insert(right.states.begin(), right.states.end());

	result.transition_functions.insert(left.transition_functions.begin(), left.transition_functions.end());
	result.transition_functions.insert(right.transition_functions.begin(), right.transition_functions.end());

	return result;
}
