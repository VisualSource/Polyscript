#pragma once
#include <string>
#include "Position.h"

class SymbolTable;

class Context {
	public:
		Context(std::string display_name, SymbolTable* scope, Context* parent = nullptr, Position parent_entry = Position());
		std::string GetDisplayName() const { return display_name; };
		Position GetPostion() const { return parent_entry; };
		Context* GetContextParent() const { return parent; };
		SymbolTable* GetScope() const { return scope; };
	private:
		std::string display_name;
		Position parent_entry;
		Context* parent = nullptr;
		SymbolTable* scope = nullptr;
};

typedef Context* CtxPtr;


