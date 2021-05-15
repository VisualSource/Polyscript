#pragma once
#include <any>
#include <string>
#include "Node.h"
#include "Token.h"

using namespace std;

class ListAccessNode : public Node {
	public:
		ListAccessNode(Token variable, any index, const Position& start, const Position& end);
		string GetVarName() const;
		any GetIndex() const;
	private:
		Token varName;
		any index;
};

