#pragma once
#include<exception>

using namespace std;
namespace moon {

	class log_negtive :public exception {
	
	public:
		log_negtive() :exception("���ܶԸ���ȡ������") {}
	};

	class division_zero :public exception {
	public:
		division_zero():exception("0������Ϊ������") {}
	};

	class definition_error :public exception {
	public:
		definition_error() :exception("�������/����ʱ�����ж����=���ţ�") {}
	};

	class definition_brakect :public exception {
	public:
		definition_brakect() :exception("�������/����ʱ���Ų�ƥ�䣡") {}
	};

	class arc_range :public exception {
	public:
		arc_range() :exception("�����Ǻ�����������ֵ���ܳ���1��") {}
	};

	class undefined_str :public exception {
	public:
		undefined_str() :exception("δ����ķ��ţ�") {}
	};
}