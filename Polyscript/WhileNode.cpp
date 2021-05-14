#include "WhileNode.h"

WhileNode::WhileNode(any conditon, any body): conditon_node(conditon), body_node(body)
{
	// TODO make function to get postion from node
	start = Position(); // from condition
	end = Position(); // from end
}

Position WhileNode::GetStart() const
{
	return start;
}

Position WhileNode::GetEnd() const
{
	return end;
}

any WhileNode::GetConditionNode() const
{
	return conditon_node;
}

any WhileNode::GetBodyNode() const
{
	return body_node;
}
