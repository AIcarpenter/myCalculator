#pragma once

#include"selfDefinitionOperator.h"
#include<algorithm>
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<queue>
#include<stack>
#include<set>

using namespace std;

namespace moon {
	enum expressionType { function_definition,variable_definition,pure_expression,commands};
	
	const size_t max_identifier_len = 6;

	//sin ,cos ,tan ,arcsin ,arccos ,arctan
	//log , ln , lg
	map<string, int> base_ops{ 
		{",",0},
		{ "+", 1 },{ "-", 1 },
		{ "*", 2 },{ "/", 2 },{"log",2},{ "ln",2 },{ "lg",2 },{ "sin",2 },{ "cos",2 },{ "tan",2 },{ "arcsin",2 },{ "arccos",2 },{ "arctan",2 },
		{ "(", 3 },{ ")", 3 } };
	
	map<string, int> self_define_ops{};

	map<string, moon::selfDefinitionOperator> hash_self_define_ops{};
	map<string, moon::selfDefinitionOperator> hash_self_define_variables{};

	/*
		judge if the pattern can be prefix of the key operators/functions 
		return value:				1->完全匹配
										0->前缀匹配
									   -1->匹配失败
	*/
	int _match_prefix(string pattern, string text) {
		if (pattern == text) return 1;
		size_t index = 0;
		int flag = -1;
		for (; index != pattern.size(); ++index) {
			if (pattern[index] == text[index])
				flag = 0;
			else
				break;
		}
		return flag;
	}

	int match_prefix(string pattern) {
		set<int> result;
		for (auto &entry : base_ops) {
			if (entry.first.length() >= pattern.length()) {
				/*auto tem = _match_prefix(pattern, entry.first);
				if (tem == 1) return 1;
				else if (tem == 0) return 0;*/
				auto tem = _match_prefix(pattern, entry.first);
				result.insert(tem);
			}
		}
		for (auto &entry : hash_self_define_ops) {
			if (entry.first.length() >= pattern.length()) {
				//auto tem = _match_prefix(pattern, entry.first);
				//if (tem == 1) return 1;
				//else if (tem == 0) return 0;
				result.insert(_match_prefix(pattern, entry.first));
			}
		}
		for (auto &entry : hash_self_define_variables) {
			if (entry.first.length() >= pattern.length()) {
				//auto tem = _match_prefix(pattern, entry.first);
				//if (tem == 1) return 1;
				//else if (tem == 0) return 0;
				result.insert(_match_prefix(pattern, entry.first));
			}
		}
		if (result.find(1) != result.end()) return 1;
		else if (result.find(0) != result.end()) return 0;
		else	return -1;
	}
	class parseMachine {
	private:
		 string raw_text;
		queue<string> lexical_units;

	public:
		parseMachine::parseMachine(string text) :raw_text(text) {}
		moon::expressionType parse_to_units();
		
		moon::value_type getResult();
		void show_text();
	};


	/*
		show the split result of the pure expression
	*/
	void moon::parseMachine::show_text() {
		while (!lexical_units.empty()) {
			cout << lexical_units.front() << " ";
			lexical_units.pop();
		}
	}

	/*
		parse the expression, and told the caller whether this is a function definition,
			variable definition or pure expression
	*/
	moon::expressionType moon::parseMachine::parse_to_units() {
		if (raw_text.find("=") == string::npos) {
			//this is a pure expression
			//split it only
			size_t pos = 0;
			string first_buffer, second_buffer;
			while(pos!=raw_text.size()){
				first_buffer.push_back(raw_text[pos]); 
				++pos;
				auto tem = match_prefix(first_buffer);
				if (tem == -1) {//完全不匹配，是数字，或者输入非法
					second_buffer.append(first_buffer); first_buffer.clear();
				}
				else if (tem == 1){//完全匹配，运算符或函数符号
					if (!second_buffer.empty()) {
						//auto tem = match_prefix(lexical_units.back());
						//if (tem == -1) lexical_units.push("*");
						lexical_units.push(second_buffer); second_buffer.clear();
					}
					lexical_units.push(first_buffer);
					first_buffer.clear();
				}
			}
			if (!second_buffer.empty())
			{
				//auto tem = match_prefix(lexical_units.back());
				//if (tem == -1) lexical_units.push("*");
				lexical_units.push(second_buffer);
			}return expressionType::pure_expression;
		}
		else {
			//this is a function or variable
			selfDefinitionOperator tem(raw_text);
			if (tem.getType() == defineType::function) {
				hash_self_define_ops[tem.getSyntax()] = std::move(tem);
				return expressionType::function_definition;
			}
			else {
				hash_self_define_variables[tem.getSyntax()] = std::move(tem);
				return expressionType::variable_definition;
			}
		}
	}

}