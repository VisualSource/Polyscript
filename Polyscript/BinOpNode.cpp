#include "BinOpNode.h"
#include "NodeUtils.h"

BinOpNode::BinOpNode(any left_node, Token op_token, any right_node): left_node(left_node), op_token(op_token), right_node(right_node) {
    start = op_token.GetStart().value();
    end = op_token.GetEnd().value();
}

Token BinOpNode::GetOpToken() const {
    return op_token;
}

any BinOpNode::GetRightNode() const {
    return right_node;
}

any BinOpNode::GetLeftNode() const {
    return left_node;
}

std::ostream& operator<<(std::ostream& lhs, const BinOpNode& token) {
      
    lhs << "(";
    NodeUtils::printNode(lhs, token.left_node);
    lhs << "," << token.op_token << ", ";
    NodeUtils::printNode(lhs, token.right_node);
    lhs << ")";

    return lhs;
}
