#pragma once
#include <string>
#include <variant>
#include "Types.h"

namespace ScopeTypes {
	typedef std::variant<Null, Integer, Float, Function, String, List, BuiltInFunction, Enum, EnumValue, Namespace, Object> Var;
	struct Node {
		std::string key;
		bool writable;
		Var value;
		Node* next;
	};
	typedef Node* NodePtr;
	template<typename T>
	bool isType(const Var& type){
		return std::get_if<T>(&type) != nullptr;
	}
}
