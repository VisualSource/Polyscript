#pragma once
#include "BaseFunction.h"

using namespace std;

class Function: public BaseFunction
{
	public:
		Function(any body_node, vector<string> arg_names, string name = "<anonymous>");
		virtual any exceute(vector<any> args) override;
	private:
		any body_node;
		vector<string> arg_names;
};

