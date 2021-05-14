#include "VarAccessNode.h"

VarAccessNode::VarAccessNode(Token var_token): var_token(var_token)
{
	start = var_token.GetStart().value();
	end = var_token.GetEnd().value();
}

Token VarAccessNode::GetToken() const
{
	return var_token;
}

Position VarAccessNode::GetStart() const
{
	return start;
}

Position VarAccessNode::GetEnd() const
{
	return start;
}

std::ostream& operator<<(std::ostream& lhs, const VarAccessNode& token)
{
	lhs << token.var_token;
	return lhs;
}
