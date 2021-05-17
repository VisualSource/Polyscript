#pragma once
#include <any>
#include <ostream>

using namespace std;

namespace InterTypes {

	template<typename T>
	bool isTypeOf(const any& node) {
		return node.type() == typeid(T);
	};
	void print(ostream& rhs, const any& node);
}