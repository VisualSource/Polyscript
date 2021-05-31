#pragma once
#include <optional>
#include <any>
#include "Token.h"
#include "Node.h"

class VarAssignNode: public Node
{
	public:
		VarAssignNode(Token var_token, bool constant, std::any node, Position start, Position end, std::optional<Token> varType = std::nullopt);
		Token GetVarType() const;
		std::any GetNode() const;
		bool isConst() const;
		Token GetToken() const;
		friend std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token);
	private:
		std::optional<Token> type;
		std::any node;
		Token var_token;
		bool constant = false;
};

