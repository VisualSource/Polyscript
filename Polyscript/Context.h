#pragma once
#include <string>
#include "Position.h"

using namespace std;

class SymbolTable;

class Context {
	public:
		Context(string display_name, SymbolTable* scope, Context* parent = nullptr, Position parent_entry = Position());
		string GetDisplayName() const { return display_name; };
		Position GetPostion() const { return parent_entry; };
		Context* GetContextParent() const { return parent; };
		SymbolTable* GetScope() const { return scope; };
	private:
		string display_name;
		Position parent_entry;
		Context* parent = nullptr;
		SymbolTable* scope = nullptr;
};

typedef Context* CtxPtr;


