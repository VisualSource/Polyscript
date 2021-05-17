#pragma once
#include <ostream>
#include <any>


using namespace std;

class Position;


namespace NodeUtils {

	template<typename T>
	bool isNode(const any& node) {
		return node.type() == typeid(T);
	}
	ostream& printNode(std::ostream& lhs, const any& node);
}
