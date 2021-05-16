#include "ReturnNode.h"

ReturnNode::ReturnNode(any return_value, Position start, Position end): return_value(return_value)
{
	SetStart(start);
	SetEnd(end);
}

any ReturnNode::GetReturnValue() const
{
	return return_value;
}
