#pragma once

#include"calculateException.h"
#include<algorithm>
#include<iostream>
#include<string>
#include<map>
#include<vector>
using namespace std;

//TODO LIST:
//1.暂时不能解决多重变量定义问题：a=1,b=a;
//2.用户计算含有自定义函数的表达式时，必须检查函数参数是否匹配

namespace moon {
	typedef double value_type ;

	enum defineType {variable,function};

	vector<string> split(string text, string split_char) {
		vector<string> result;
		for (string::size_type pos_begin = 0; true;) {
			auto pos_end = text.find(split_char, pos_begin + 1);
			result.push_back(text.substr(pos_begin, pos_end - pos_begin));
			if (pos_end == string::npos) break;
			else pos_begin = pos_end + 1;
		}
		return std::move(result);
	}

	class selfDefinitionOperator {
	private:
		defineType tag;
		string syntax;
		string expression;
		size_t argc;
		map<string, value_type> argv;
	public:
		selfDefinitionOperator(){}
		selfDefinitionOperator(string text) {
			if (count(text.begin(), text.end(), '=') != 1) throw definition_error();
			size_t pos_of_equal = text.find("=");
			size_t pos_of_left_bracket = text.find("(");	
			if (pos_of_left_bracket == string::npos) {
				//this is a variable 
				tag = variable;
				if (count(text.begin(), text.end(), ')')!=0) throw definition_brakect();
				syntax = text.substr(0, pos_of_equal);
				expression = text.substr(pos_of_equal + 1);
			}
			else {
				//this is a function
				tag = function;
				if (count(text.begin(),text.end(),')')==0) throw definition_brakect();
				size_t pos_of_right_bracket = text.find(")", pos_of_left_bracket);
				syntax = text.substr(0, pos_of_left_bracket);
				expression = text.substr(pos_of_equal+1);
				//now deal with arguments
				string arguments = text.substr(pos_of_left_bracket + 1, pos_of_right_bracket - pos_of_left_bracket-1);
				vector<string> syntax_of_args = split(arguments, ",");
				for (auto &key : syntax_of_args)
					argv[key];
				argc = syntax_of_args.size();
			}
		}
		void show_for_debug() {
			if (tag == variable) {
				cout << "---------------------" << endl;
				cout << "syntax: " << syntax << endl;
				cout << "expression: " << expression << endl;
				cout << "---------------------" << endl;
			}else {
				cout << "---------------------" << endl;
				cout << "syntax: " << syntax << endl;
				cout << "arguments: ";
				for (auto &x : argv) cout << x.first << " ";
				cout << endl;
				cout << "expression: " << expression << endl;
				cout << "---------------------" << endl;

			}
		}
		value_type do_mathmatics(vector<value_type> values) {
		}
		string getSyntax() { return syntax; }
		defineType getType() {
			return tag;
		}


	};
}