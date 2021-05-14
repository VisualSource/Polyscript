#include "VarAssignNode.h"

VarAssignNode::VarAssignNode(Token var_token, any node, optional<Token> varType): var_token(var_token), node(node)
{
	start = var_token.GetStart().value();


	if (varType.has_value()) {
		type = varType.value().GetType();
		end = varType.value().GetEnd().value();
	}
	else {
		type = var_token.GetType();
		end = var_token.GetEnd().value();
	}
}

TypeToken VarAssignNode::GetVarType() const
{
	return type;
}

any VarAssignNode::GetNode() const
{
	return node;
}

Token VarAssignNode::GetToken() const
{
	return var_token;
}

Position VarAssignNode::GetStart() const
{
	return start;
}

Position VarAssignNode::GetEnd() const
{
	return end;
}

std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token)
{
	lhs << "(" <<token.var_token << ", ";
	NodeUtils::printNode(lhs, token.node);
	lhs << ")";

	return lhs;
}
