#pragma once
#include <variant>
#include <string>
#include "Integer.h"
#include "Float.h"
#include "Function.h"
#include "String.h"
#include "List.h"
#include "BuiltInFunction.h"

using namespace std;

namespace ScopeTypes {
	struct  Node;
	typedef variant<monostate, Integer, Float, Function, String, List, BuiltInFunction> Var;
	typedef Node* NodePtr;
	bool isInteger(const Var& value);
	bool isFloat(const Var& value);
	bool isString(const Var& value);
	bool isFunction(const Var& value);
	bool isString(const Var& value);
	bool isList(const Var& value);
	bool isBuiltIn(const Var& value);
}


const string HEAD_OF_LIST = "-1";

class SymbolTable {
	public:
		SymbolTable();
		SymbolTable(string secKey, ScopeTypes::Var value);
		SymbolTable(const SymbolTable& lhs);
		~SymbolTable();
		bool isEmpty() const;
		bool hasVar(string key) const;
		ScopeTypes::Var get(string key) const;
		void setValue(string key, ScopeTypes::Var value, bool searchParent = true);
		void add(string key, ScopeTypes::Var value, bool serachParent = true);
		void setParent(SymbolTable* parent);
	private:
		SymbolTable* parent = nullptr;
		ScopeTypes::NodePtr head = nullptr;
		void insert(string key, ScopeTypes::Var value);
		void remove(string key);
};

