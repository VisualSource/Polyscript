#include "NumberNode.h"

NumberNode::NumberNode(Token token): token(token) {
	SetStart(token.GetStart().value());
	SetEnd(token.GetEnd().value());
}

Token NumberNode::GetToken() const {
	return token;
}

std::ostream& operator<<(std::ostream& lhs, const NumberNode& token) {
	lhs << token.token;
	return lhs;
}
