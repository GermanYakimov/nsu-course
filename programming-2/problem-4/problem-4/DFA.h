#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

struct pair_hash {
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2>& p) const {
		auto h1 = hash<T1>{}(p.first);
		auto h2 = hash<T2>{}(p.second);

		return h1 ^ h2;
	}
};

typedef unordered_multimap<char, pair<string, string>> map_f_nfa;
typedef unordered_map<pair<char, string>, string, pair_hash> map_f_dfa;
typedef unordered_map<string, bool> map_s;

class DFA
{
	map_s states;
	map_f_dfa transition_functions;
	string initial_state;
	unordered_set<char> alphabet;

	struct NFA
	{
		map_s states;
		map_f_nfa transition_functions;
		string initial_state;
		unordered_set<char> alphabet;
	};

	static DFA nfa_to_dfa(DFA::NFA nfa);

	static DFA::NFA e_nfa_to_nfa(DFA::NFA e_nfa);

	static bool is_e_nfa(DFA::NFA e_nfa);

	static DFA determine(DFA::NFA e_nfa);

	string step(string state, char c);

	static DFA enumerate_states(DFA a, size_t start);

	static unordered_set<string> possible_states_e(string start, unordered_set<string> buffer, DFA::NFA e_nfa);

	static DFA build_fa(string regexp);

	static unordered_set<string> possible_states_next_step(char c, vector<string> states_list, DFA::NFA e_nfa);

	void rename_state(string from, string to);

	static DFA kleene_star(DFA a);

	DFA::NFA to_nfa();

public:
	DFA() {}

	DFA(const DFA& that);

	static DFA build_from_stream(istream& input);

	static DFA build_from_regexp(string regexp);

	void print();

	string recognize(string source);

	friend DFA operator&(DFA left, DFA right);

	friend DFA operator|(DFA left, DFA right);

	friend DFA::NFA operator+(DFA::NFA left, DFA::NFA right);

};
