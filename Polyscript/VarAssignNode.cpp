#include "VarAssignNode.h"
#include "NodeUtils.h"

VarAssignNode::VarAssignNode(Token var_token, any node, optional<Token> varType): var_token(var_token), node(node)
{
	SetStart(var_token.GetStart().value());


	if (varType.has_value()) {
		type = varType.value();
		SetEnd(varType.value().GetEnd().value());
	}
	else {
		type = nullopt;
		SetEnd(NodeUtils::GetEndFromNode(node));
	}
}

Token VarAssignNode::GetVarType() const
{
	if (type.has_value()) {
		return type.value();
	}
	return var_token;
}

any VarAssignNode::GetNode() const
{
	return node;
}

Token VarAssignNode::GetToken() const
{
	return var_token;
}

std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token)
{
	lhs << "(" <<token.var_token << ", ";
	NodeUtils::printNode(lhs, token.node);
	lhs << ")";

	return lhs;
}
