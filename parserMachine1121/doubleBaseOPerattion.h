#pragma once
#include"calculateException.h"
#include<cmath>
#include<vector>
#include<string>
//-this head file is for base operatortions of dobule
//---cause double has some operators already, like +,-,*,/...
//---i just define sin,cos,tan,arcsin,arccos,arctan,log,lg,ln,^
//--- we don't care about operators % here.

using namespace std;
namespace moon {
	double do_base_mathmatics(vector<double> &values, string operation) {
		if (operation == "+") return values[0] + values[1];
		if (operation == "-") return values[0] - values[1];
		if (operation == "*") return values[0] * values[1];
		if (operation == "/") {
			if (values[1] == 0)
				throw division_zero();
			else
				return values[0] / values[1];
		}
		if (operation == "^") return pow(values[0], values[1]);
		if (operation == "sin") return sin(values[0]);
		if (operation == "cos") return cos(values[0]);
		if (operation == "tan") return tan(values[0]);
		if (operation == "arcsin") { 
			if (values[0] > 1 || values[0] < -1) throw arc_range();
			return asin(values[0]); }
		if (operation == "arccos") {
			if (values[0] > 1 || values[0] < -1) throw arc_range();
			return acos(values[0]);
	}
		if (operation == "arctan") return atan(values[0]);

		if (values[0] < 0) throw log_negtive();
		if (operation == "log") return log2(values[0]);
		if (operation == "lg") return log10(values[0]);
		if (operation == "ln") return log(values[0]);

	}

}