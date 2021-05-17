#include "VarReasignNode.h"

VarReasignNode::VarReasignNode(Token ident, Token op, any expr, Position start, Position end): Node(start,end), identifer(ident), opeartor(op), expr(expr)
{
}

Token VarReasignNode::GetIdentifer() const
{
	return identifer;
}

Token VarReasignNode::GetOperationToken() const
{
	return opeartor;
}

any VarReasignNode::GetExpr() const
{
	return expr;
}
