#include "NumberNode.h"

NumberNode::NumberNode(Token token, Position start, Position end): Node(start,end), token(token) {
}

Token NumberNode::GetToken() const {
	return token;
}

std::ostream& operator<<(std::ostream& lhs, const NumberNode& token) {
	lhs << token.token;
	return lhs;
}
