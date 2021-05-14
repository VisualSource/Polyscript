#include "NodeUtils.h"


ostream& NodeUtils::printNode(std::ostream& lhs, const any& node) {

	if (NodeUtils::isNumberNode(node)) {
		lhs << any_cast<NumberNode>(node);
	} else if (NodeUtils::isBinOpNode(node)) {
		lhs << any_cast<BinOpNode>(node);
	} else if(NodeUtils::isUnaryOpNode(node)){
		lhs << any_cast<UnaryOpNode>(node);
	}
	else if (NodeUtils::isVarAccessNode(node)) {
		lhs << any_cast<VarAccessNode>(node);
	}
	else if (NodeUtils::isAssignNode(node)) {
		lhs << any_cast<VarAssignNode>(node);
	}

	return lhs;
}

bool NodeUtils::isBinOpNode(const any& node) {
	return node.type() == typeid(BinOpNode);
}

bool NodeUtils::isNumberNode(const any& node){
	return node.type() == typeid(NumberNode);
}

bool NodeUtils::isUnaryOpNode(const any& node) {
	return node.type() == typeid(UnaryOpNode);
}

bool NodeUtils::isVarAccessNode(const any& node)
{
	return node.type() == typeid(VarAccessNode);
}

bool NodeUtils::isAssignNode(const any& node)
{
	return node.type() == typeid(VarAssignNode);
}

bool NodeUtils::isIfNode(const any& node)
{
	return node.type() == typeid(IfNode);
}
