#pragma once
#include <optional>
#include <any>
#include "Token.h"
#include "Node.h"

using namespace std;

class VarAssignNode: public Node
{
	public:
		VarAssignNode(Token var_token, any node, optional<Token> varType = nullopt);
		Token GetVarType() const;
		any GetNode() const;
		Token GetToken() const;
		friend std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token);
	private:
		optional<Token> type;
		any node;
		Token var_token;
};

