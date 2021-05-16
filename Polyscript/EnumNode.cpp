#include "EnumNode.h"

EnumNode::EnumNode(string name, const vector<Token>& values, Position start, Position end): keys(values), name(name)
{
	SetStart(start);
	SetEnd(end);
}

vector<Token> EnumNode::GetKeys() const
{
	return keys;
}
