#include "VarAccessNode.h"

VarAccessNode::VarAccessNode(Token var_token): var_token(var_token)
{
	SetStart(var_token.GetStart().value());
	SetEnd(var_token.GetEnd().value());
}

Token VarAccessNode::GetToken() const
{
	return var_token;
}

std::ostream& operator<<(std::ostream& lhs, const VarAccessNode& token)
{
	lhs << token.var_token;
	return lhs;
}
