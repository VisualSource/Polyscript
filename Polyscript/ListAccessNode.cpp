#include "ListAccessNode.h"

ListAccessNode::ListAccessNode(Token variable, any index, const Position& start, const Position& end): varName(variable), index(index)
{
	SetStart(start);
	SetEnd(end);
}

string ListAccessNode::GetVarName() const
{
	return varName.GetValue().value();
}

any ListAccessNode::GetIndex() const
{
	return index;
}
