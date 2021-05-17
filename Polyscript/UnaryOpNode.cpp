#include "UnaryOpNode.h"
#include "NodeUtils.h"

std::ostream& operator<<(std::ostream& lhs, const UnaryOpNode& token) {

    lhs << "(" << token.GetToken() << ", ";
    NodeUtils::printNode(lhs,token.GetNode());
    lhs << ")";
    return lhs;
}

UnaryOpNode::UnaryOpNode(Token op_tok, any node, Position start, Position end): Node(start, end), op_tok(op_tok), node(node) {
}

Token UnaryOpNode::GetToken() const {
    return op_tok;
}

any UnaryOpNode::GetNode() const {
    return node;
}
