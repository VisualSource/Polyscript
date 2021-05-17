#include "NodeUtils.h"
#include "Nodes.h"
#include "Position.h"


ostream& NodeUtils::printNode(std::ostream& lhs, const any& node) {
	using NodeUtils::isNode;
	if (isNode<NumberNode>(node)) {
		lhs << any_cast<NumberNode>(node);
	} else if (isNode<BinOpNode>(node)) {
		lhs << any_cast<BinOpNode>(node);
	} else if(isNode<UnaryOpNode>(node)){
		lhs << any_cast<UnaryOpNode>(node);
	}
	else if (isNode<VarAccessNode>(node)) {
		lhs << any_cast<VarAccessNode>(node);
	}
	else if (isNode<VarAssignNode>(node)) {
		lhs << any_cast<VarAssignNode>(node);
	}
	else if (isNode<StringNode>(node)) {
		lhs << any_cast<StringNode>(node);
	}

	return lhs;
}

