#include"selfDefinitionOperator.h"
#include"doubleBaseOPerattion.h"
#include"parseMachine.h"
#include<string>
#include<vector>
int main() {
	string text("1+2");
	while (true) {
		text.clear();
		cin >> text;
		moon::parseMachine tem(text);
		tem.parse_to_units();
		tem.show_text();
		cout << endl;
	}

	//moon::selfDefinitionOperator op("f(x)=1+2");
	//op.show_for_debug();

	//vector<double> argv;
	//argv.push_back(0.5);
	//argv.push_back(3);
	//try {
	//	cout << moon::do_base_mathmatics(argv, string("+")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("-")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("*")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("/")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("sin")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("cos")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("tan")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("arcsin")) << endl;

	//	cout << moon::do_base_mathmatics(argv, string("arccos")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("arctan")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("log")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("ln")) << endl;
	//	cout << moon::do_base_mathmatics(argv, string("lg")) << endl;


	//}
	//catch (exception &x) {
	//	cout << x.what() << endl;
	//}
}