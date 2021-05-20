#include "ReturnNode.h"

using namespace std;

ReturnNode::ReturnNode(any return_value, Position start, Position end): Node(start,end), return_value(return_value)
{
}

any ReturnNode::GetReturnValue() const
{
	return return_value;
}
