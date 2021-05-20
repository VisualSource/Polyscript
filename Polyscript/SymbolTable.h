#pragma once
#include <string>
#include "SymbolTableUtils.h"


class Context;
const std::string HEAD_OF_LIST = "-1";

class SymbolTable {
	public:
		SymbolTable();
		SymbolTable(std::string secKey, ScopeTypes::Var value);
		SymbolTable(const SymbolTable& lhs);
		~SymbolTable();
		bool isEmpty() const;
		bool hasVar(std::string key) const;
		ScopeTypes::Var get(std::string key, Context* context) const;
		void set(std::string key, ScopeTypes::Var value, Context* context);
		void add(std::string key, ScopeTypes::Var value, Context* context);
		void setParent(SymbolTable* parent);
	private:
		SymbolTable* parent = nullptr;
		ScopeTypes::NodePtr head = nullptr;
		void insert(std::string key, ScopeTypes::Var value);
		void remove(std::string key);
};

