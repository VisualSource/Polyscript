#include "WhileNode.h"
#include "NodeUtils.h"

using namespace std;

WhileNode::WhileNode(any conditon, any body, Position start, Position end): Node(start,end), conditon_node(conditon), body_node(body)
{
}

any WhileNode::GetConditionNode() const
{
	return conditon_node;
}

any WhileNode::GetBodyNode() const
{
	return body_node;
}
