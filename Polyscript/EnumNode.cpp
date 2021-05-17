#include "EnumNode.h"

EnumNode::EnumNode(string name, const vector<Token>& values, Position start, Position end): Node(start,end), keys(values), name(name)
{
}

vector<Token> EnumNode::GetKeys() const
{
	return keys;
}
