#pragma once
#include <vector>
#include "Node.h"
#include "Token.h"

class EnumNode : public Node {
	public:
		EnumNode(std::string name, const std::vector<Token>& values, Position start, Position end);
		std::vector<Token> GetKeys() const;
		std::string GetName() const { return name; };
	private:
		std::string name = "";
		std::vector<Token> keys;
};

