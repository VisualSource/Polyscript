#pragma once
#include <optional>
#include <any>
#include "Position.h"
#include "Token.h"
#include "NodeUtils.h"

using namespace std;

class VarAssignNode
{
	public:
		VarAssignNode(Token var_token, any node, optional<Token> varType = nullopt);
		TypeToken GetVarType() const;
		any GetNode() const;
		Token GetToken() const;
		Position GetStart() const;
		Position GetEnd() const;
		friend std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token);
	private:
		TypeToken type;
		any node;
		Token var_token;
		Position start;
		Position end;
};

