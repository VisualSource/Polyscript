#pragma once
#include "BaseFunction.h"

using namespace std;


class BuiltInFunction : public BaseFunction {
	public:
		BuiltInFunction(string name, vector<string> args_names);
		virtual any exceute(vector<any> args) override;
	private:
		vector<string> args_names;
		
};

