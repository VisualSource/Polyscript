#pragma once
#include <any>
#include <ostream>

namespace InterTypes {

	template<typename T>
	bool isTypeOf(const std::any& node) {
		return node.type() == typeid(T);
	};
	void print(std::ostream& rhs, const std::any& node);
}