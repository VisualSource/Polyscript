#include "StringNode.h"

StringNode::StringNode(Token token): token(token)
{
}

Token StringNode::GetToken() const
{
	return token;
}

std::ostream& operator<<(std::ostream& lhs, const StringNode& token)
{
	lhs << token.token.GetValue().value();
	return lhs;
}
