#include "VarReasignNode.h"

VarReasignNode::VarReasignNode(Token ident, Token op, any expr, Position start, Position end): identifer(ident), opeartor(op), expr(expr)
{
	SetStart(start);
	SetEnd(end);
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
