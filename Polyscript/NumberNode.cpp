#include "NumberNode.h"

NumberNode::NumberNode(Token token): token(token) {
	start = token.GetStart().value();
	end = token.GetEnd().value();
}

Token NumberNode::GetToken() const {
	return token;
}

std::ostream& operator<<(std::ostream& lhs, const NumberNode& token) {
	lhs << token.token;
	return lhs;
}
