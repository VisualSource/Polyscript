#include "VarAssignNode.h"
#include "NodeUtils.h"

using namespace std;

VarAssignNode::VarAssignNode(Token var_token, bool constant, any node, Position start, Position end, optional<Token> varType): Node(start,end), var_token(var_token), node(node), constant(constant)
{
	if (varType.has_value()) {
		type = varType.value();
	} else {
		type = nullopt;
	}
}

Token VarAssignNode::GetVarType() const
{
	if (type.has_value()) {
		return type.value();
	}
	return var_token;
}

any VarAssignNode::GetNode() const
{
	return node;
}

bool VarAssignNode::isConst() const
{
	return constant;
}

Token VarAssignNode::GetToken() const
{
	return var_token;
}

std::ostream& operator<<(std::ostream& lhs, const VarAssignNode& token)
{
	lhs << "(" <<token.var_token << ", ";
	NodeUtils::printNode(lhs, token.node);
	lhs << ")";

	return lhs;
}
