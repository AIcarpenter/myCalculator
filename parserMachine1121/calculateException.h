#pragma once
#include<exception>

using namespace std;
namespace moon {

	class log_negtive :public exception {
	
	public:
		log_negtive() :exception("不能对负数取对数！") {}
	};

	class division_zero :public exception {
	public:
		division_zero():exception("0不能作为除数！") {}
	};

	class definition_error :public exception {
	public:
		definition_error() :exception("定义变量/函数时不能有多个“=”号！") {}
	};

	class definition_brakect :public exception {
	public:
		definition_brakect() :exception("定义变量/函数时括号不匹配！") {}
	};

	class arc_range :public exception {
	public:
		arc_range() :exception("反三角函数参数绝对值不能超过1！") {}
	};

	class undefined_str :public exception {
	public:
		undefined_str() :exception("未定义的符号！") {}
	};
}