#include "WhileNode.h"
#include "NodeUtils.h"

WhileNode::WhileNode(any conditon, any body): conditon_node(conditon), body_node(body)
{
	SetStart(NodeUtils::GetStartFromNode(conditon));
	if (body.has_value()) {
		SetEnd(NodeUtils::GetEndFromNode(body));
	}
	else {
		SetEnd(NodeUtils::GetEndFromNode(conditon));
	}
	
}

any WhileNode::GetConditionNode() const
{
	return conditon_node;
}

any WhileNode::GetBodyNode() const
{
	return body_node;
}
