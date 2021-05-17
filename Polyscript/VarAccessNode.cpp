#include "VarAccessNode.h"

VarAccessNode::VarAccessNode(Token var_token, Position start, Position end): Node(start,end), var_token(var_token)
{
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
