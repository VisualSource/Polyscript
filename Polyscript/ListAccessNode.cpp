#include "ListAccessNode.h"

using namespace std;

ListAccessNode::ListAccessNode(Token variable, any index, const Position& start, const Position& end): Node(start,end), varName(variable), index(index)
{
}

string ListAccessNode::GetVarName() const
{
	return varName.GetValue().value();
}

any ListAccessNode::GetIndex() const
{
	return index;
}
