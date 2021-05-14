#include "UnaryOpNode.h"
#include "NodeUtils.h"

std::ostream& operator<<(std::ostream& lhs, const UnaryOpNode& token) {

    lhs << "(" << token.GetToken() << ", ";
    NodeUtils::printNode(lhs,token.GetNode());
    lhs << ")";
    return lhs;
}

UnaryOpNode::UnaryOpNode(Token op_tok, any node): op_tok(op_tok), node(node) {
    start = op_tok.GetStart().value();
    end = op_tok.GetEnd().value();
}

Token UnaryOpNode::GetToken() const {
    return op_tok;
}

any UnaryOpNode::GetNode() const {
    return node;
}
