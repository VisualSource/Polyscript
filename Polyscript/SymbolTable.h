#pragma once
#include <variant>
#include <string>
#include "Integer.h"
#include "Float.h"

using namespace std;


struct  Node;
typedef variant<monostate, Integer, Float> Var;
typedef Node* NodePtr;


const string HEAD_OF_LIST = "-1";

class SymbolTable {
	public:
		SymbolTable();
		SymbolTable(string secKey, Var value);
		SymbolTable(const SymbolTable& lhs);
		~SymbolTable();
		bool isEmpty() const;
		bool hasVar(string key) const;
		Var get(string key) const;
		void setValue(string key, Var value, bool searchParent = true);
		void add(string key, Var value, bool serachParent = true);
		void setParent(SymbolTable* parent);
	private:
		SymbolTable* parent = nullptr;
		NodePtr head = nullptr;
		void insert(string key, Var value);
		void remove(string key);
};

