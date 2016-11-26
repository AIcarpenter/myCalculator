#pragma once

#include"selfDefinitionOperator.h"
#include"doubleBaseOPerattion.h"
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
	const int self_definition_priority = 3;
	const value_type invalid_value = 0xffffffff;
	//sin ,cos ,tan ,arcsin ,arccos ,arctan,log , ln , lg
	map<string, int> base_ops{ 
		{",",0},
		{ "+", 1 },{ "-", 1 },
		{ "*", 2 },{ "/", 2 },
		{"log",3},{ "ln",3 },{ "lg",3 },{ "sin",3 },{ "cos",3 },{ "tan",3 },{ "arcsin",3 },{ "arccos",3 },{ "arctan",3 },
		{"^",4},
		{ "(", 5 },{ ")", 5 } };
	
	set<string> base_functions{  "log", "ln", "lg", "sin", "cos", "tan", "arcsin", "arccos", "arctan"};
	map<string, int> self_define_ops{};

	map<string, moon::selfDefinitionOperator> hash_self_define_ops{};
	map<string, moon::selfDefinitionOperator> hash_self_define_variables{};

	value_type get_subexpression_result(string sub_expression);

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
						auto tem = _match_prefix(pattern, entry.first);
				result.insert(tem);
			}
		}
		for (auto &entry : hash_self_define_ops) {
			if (entry.first.length() >= pattern.length()) {
				result.insert(_match_prefix(pattern, entry.first));
			}
		}
		for (auto &entry : hash_self_define_variables) {
			if (entry.first.length() >= pattern.length()) {
				result.insert(_match_prefix(pattern, entry.first));
			}
		}
		if (result.find(1) != result.end()) return 1;
		else if (result.find(0) != result.end()) return 0;
		else	return -1;
	}
	
	/*
		return the priority of op
		return value:			{0,1,2,3}-->正常
									-1		   -->自定义变量
					   throw exception-->未定义的字符
	*/
	int get_priority(string op) {
		{
			auto ite = base_ops.find(op);
			if (ite != base_ops.end())
				return ite->second;
		}
		{
			auto ite = hash_self_define_ops.find(op);
			if (ite != hash_self_define_ops.end())
				return self_definition_priority;
		}
		{
			auto ite = hash_self_define_variables.find(op);
			if (ite != hash_self_define_variables.end())
				return -1;
		}
		throw undefined_str();
	}

	/*
		return the parameter number the operator/function needs
	*/
	int get_parameter_number(string op) {
		auto ite = hash_self_define_ops.find(op);
		if (ite != hash_self_define_ops.end())
			return ite->second.get_argc();
		else {
			if (op == "+" || op == "-" || op == "*" || op == "/"||op=="^") return 2;
			else return 1;
		}
	}
	class parseMachine {
	private:
		 string raw_text;
		queue<string> lexical_units;
		queue<string> reverse_polish_notation;

		value_type str_to_value(string &text);
		moon::expressionType parse_to_units();
		moon::value_type parse_to_rpn();
		moon::value_type do_math();
		void show_rpn();
	public:
		parseMachine::parseMachine(string text) :raw_text(text) {}
		moon::value_type getResult();
		void show_text();

	};


	/*
		show the split result of the pure expression
	*/
	void moon::parseMachine::show_text() {
		auto x = lexical_units;
		while (!x.empty()) {
			cout << x.front() << " ";
			x.pop();
		}
		cout << endl;
	}
	
	void moon::parseMachine::show_rpn() {
		auto x = reverse_polish_notation;
		while (!x.empty()) {
			cout << x.front() << ' ';
			x.pop();
		}
		cout << endl;
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
							lexical_units.push(second_buffer); second_buffer.clear();
							if (hash_self_define_ops.find(first_buffer) != hash_self_define_ops.end() ||
								base_functions.find(first_buffer) != base_functions.end())
								lexical_units.push("*");
					}
					lexical_units.push(first_buffer);
					first_buffer.clear();
				}
			}
			if (!second_buffer.empty())
			{
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


	/*
		get the result of the expression
	*/
	value_type parseMachine::parse_to_rpn() {
		stack<string> ops;
		bool is_number = false;
		while (!lexical_units.empty()) {
			auto unit = lexical_units.front();
			try {
			//	get_priority(unit);
				if (get_priority(unit) == -1)
				{
					unit = to_string(str_to_value(unit));
					is_number = true;
				}
			}catch (exception &x) {
				is_number = true;
			}
			if (is_number) {
				reverse_polish_notation.push(unit);
				is_number = false;
			}
			else if (unit == ",") {
				;
			}
			else {
				if (ops.empty()) ops.push(unit);
				else {
					if (unit == ")") {
						while (ops.top() != "("){
							reverse_polish_notation.push(ops.top());
							ops.pop();
						}
						ops.pop();
					}
					else {
						if (get_priority(ops.top()) >= get_priority(unit)&&ops.top()!="(") {
							reverse_polish_notation.push(ops.top());
							ops.pop();
							ops.push(unit);
						}
						else
							ops.push(unit);
					}
				}
			}
			lexical_units.pop();
		}
		while (!ops.empty()) {
			reverse_polish_notation.push(ops.top());
			ops.pop();
		}
		
		return 1;
	}
	value_type parseMachine::str_to_value(string& text) {
		auto entry = hash_self_define_variables.find(text);
		if (entry!= hash_self_define_variables.end()) {
			return str_to_value(entry->second.getExpression());
		}
		try {
			return std::stof(text);
		}
		catch (exception) {
			return invalid_value;
		}
	}

	value_type parseMachine::do_math() {
		stack<value_type> result;
		while (!reverse_polish_notation.empty()) {
			auto top = reverse_polish_notation.front();
			reverse_polish_notation.pop();
			auto value = str_to_value(top);
			if (value != invalid_value)
				result.push(value);
			else {
				auto argc = get_parameter_number(top);
				vector<value_type> argv;
				while (argc--) {
					argv.insert(argv.begin(), result.top());
					result.pop();
				}
				if(hash_self_define_ops.find(top)==hash_self_define_ops.end()){
					result.push(do_base_mathmatics(std::move(argv), top));
				}else {
					auto sub_expression = hash_self_define_ops[top].full_text(argv);
					auto sub_result = get_subexpression_result(sub_expression);
					result.push(sub_result);
				}
			}
		}
		return result.top();
	}

	value_type parseMachine::getResult(){
		auto flag = parse_to_units();
		if (flag== expressionType::pure_expression) {
			parse_to_rpn();
			//show_rpn();
			return do_math();
		}
		else if (flag == expressionType::variable_definition) {
			return invalid_value;
		}
		else {
			//show_text();
			return invalid_value;
		}
	}

	/*
		get the result of the sub expression, with only base operators
	*/
	value_type get_subexpression_result(string sub_expression) {
		parseMachine tem(sub_expression);
		return tem.getResult();
	}
}

