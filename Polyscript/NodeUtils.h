#pragma once
#include <ostream>
#include <any>

class Position;

namespace NodeUtils {

	template<typename T>
	bool isNode(const std::any& node) {
		return node.type() == typeid(T);
	}
	std::ostream& printNode(std::ostream& lhs, const std::any& node);
}
