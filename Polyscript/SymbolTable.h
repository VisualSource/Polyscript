#pragma once
#include <variant>
#include <string>
#include "Types.h"

using namespace std;

namespace ScopeTypes {
	struct  Node;
	typedef variant<monostate, Integer, Float, Function, String, List, BuiltInFunction, Enum> Var;
	typedef Node* NodePtr;
	bool isInteger(const Var& value);
	bool isFloat(const Var& value);
	bool isString(const Var& value);
	bool isFunction(const Var& value);
	bool isString(const Var& value);
	bool isList(const Var& value);
	bool isBuiltIn(const Var& value);
	bool isEnum(const Var& value);
}

class Context;
const string HEAD_OF_LIST = "-1";

class SymbolTable {
	public:
		SymbolTable();
		SymbolTable(string secKey, ScopeTypes::Var value);
		SymbolTable(const SymbolTable& lhs);
		~SymbolTable();
		bool isEmpty() const;
		bool hasVar(string key) const;
		ScopeTypes::Var get(string key, Context* context) const;
		void set(string key, ScopeTypes::Var value, Context* context);
		void add(string key, ScopeTypes::Var value, Context* context);
		void setParent(SymbolTable* parent);
	private:
		SymbolTable* parent = nullptr;
		ScopeTypes::NodePtr head = nullptr;
		void insert(string key, ScopeTypes::Var value);
		void remove(string key);
};

