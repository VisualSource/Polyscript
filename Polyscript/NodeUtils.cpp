#include "NodeUtils.h"
#include "Nodes.h"
#include "Position.h"


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
	else if (NodeUtils::isStringNode(node)) {
		lhs << any_cast<StringNode>(node);
	}

	return lhs;
}

Position NodeUtils::GetStartFromNode(const any& node)
{
	using namespace NodeUtils;

	if (isBinOpNode(node)) {
		return any_cast<BinOpNode>(node).GetStart();
	}
	else if (isNumberNode(node)) {
		return any_cast<NumberNode>(node).GetStart();
	}
	else if (isUnaryOpNode(node)) {
		return any_cast<UnaryOpNode>(node).GetStart();
	}
	else if (isVarAccessNode(node)) {
		return any_cast<VarAccessNode>(node).GetStart();
	}
	else if (isAssignNode(node)) {
		return any_cast<VarAssignNode>(node).GetStart();
	}
	else if (isIfNode(node)) {
		return any_cast<IfNode>(node).GetStart();
	}else if(isWhileNode(node)){
		return any_cast<WhileNode>(node).GetStart();
	}else if(isForNode(node)){
		return any_cast<ForNode>(node).GetStart();
	}else if(isFuncDefNode(node)){
		return any_cast<FuncDefNode>(node).GetStart();
	} else if (isFuncCallNode(node)) {
		return any_cast<FuncCallNode>(node).GetStart();
	}
	else if (isStringNode(node)) {
		return any_cast<StringNode>(node).GetStart();
	}
	else if (isListNode(node)) {
		return any_cast<ListNode>(node).GetStart();
	}
	else if (isListAccessNode(node)) {
		return any_cast<ListAccessNode>(node).GetStart();
	}

	return Position();
}

Position NodeUtils::GetEndFromNode(const any& node)
{
	using namespace NodeUtils;
	if (isBinOpNode(node)) {
		return any_cast<BinOpNode>(node).GetEnd();
	}
	else if (isNumberNode(node)) {
		return any_cast<NumberNode>(node).GetEnd();
	}
	else if (isUnaryOpNode(node)) {
		return any_cast<UnaryOpNode>(node).GetEnd();
	}
	else if (isVarAccessNode(node)) {
		return any_cast<VarAccessNode>(node).GetEnd();
	}
	else if (isAssignNode(node)) {
		return any_cast<VarAssignNode>(node).GetEnd();
	}
	else if (isIfNode(node)) {
		return any_cast<IfNode>(node).GetEnd();
	}
	else if (isWhileNode(node)) {
		return any_cast<WhileNode>(node).GetEnd();
	}
	else if (isForNode(node)) {
		return any_cast<ForNode>(node).GetEnd();
	}
	else if (isFuncDefNode(node)) {
		return any_cast<FuncDefNode>(node).GetEnd();
	}
	else if (isFuncCallNode(node)) {
		return any_cast<FuncCallNode>(node).GetEnd();
	}
	else if (isStringNode(node)) {
		return any_cast<StringNode>(node).GetEnd();
	}
	else if (isListNode(node)) {
		return any_cast<ListNode>(node).GetEnd();
	}
	else if (isListAccessNode(node)) {
		return any_cast<ListAccessNode>(node).GetEnd();
	}
	return Position();
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

bool NodeUtils::isWhileNode(const any& node)
{
	return node.type() == typeid(WhileNode);
}

bool NodeUtils::isForNode(const any& node)
{
	return node.type() == typeid(ForNode);
}

bool NodeUtils::isFuncDefNode(const any& node)
{
	return node.type() == typeid(FuncDefNode);
}

bool NodeUtils::isFuncCallNode(const any& node)
{
	return node.type() == typeid(FuncCallNode);
}

bool NodeUtils::isStringNode(const any& node)
{
	return node.type() == typeid(StringNode);
}

bool NodeUtils::isListNode(const any& node)
{
	return node.type() == typeid(ListNode);
}

bool NodeUtils::isListAccessNode(const any& node)
{
	return node.type() == typeid(ListAccessNode);
}

