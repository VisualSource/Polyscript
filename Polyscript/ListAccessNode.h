#pragma once
#include <any>
#include <string>
#include "Node.h"
#include "Token.h"

class ListAccessNode : public Node {
	public:
		ListAccessNode(Token variable, std::any index, const Position& start, const Position& end);
		std::string GetVarName() const;
		std::any GetIndex() const;
	private:
		Token varName;
		std::any index;
};

