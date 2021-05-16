#pragma once
#include <vector>
#include "Node.h"
#include "Token.h"

class EnumNode : public Node {
	public:
		EnumNode(string name, const vector<Token>& values, Position start, Position end);
		vector<Token> GetKeys() const;
		string GetName() const { return name; };
	private:
		string name = "";
		vector<Token> keys;
};

