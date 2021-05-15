#pragma once
#include <any>
#include <ostream>

using namespace std;

namespace InterTypes {
	bool isFloat(const any& node);
	bool isInteger(const any& node);
	bool isFunction(const any& node);
	bool isString(const any& node);
	bool isList(const any& node);
	bool isBuiltin(const any& node);
	void print(ostream& rhs, const any& node);
}