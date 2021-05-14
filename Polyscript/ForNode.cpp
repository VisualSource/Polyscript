#include "ForNode.h"

ForNode::ForNode(Token var_token_name, any start, any end, any body, any step): var_token_name(var_token_name), start_value_node(start), end_value_node(end), body_node(body), step(step)
{
	start = var_token_name.GetStart().value();
	end = var_token_name.GetEnd().value(); // TODO get end from body
}

Token ForNode::GetVarTokenName() const
{
	return var_token_name;
}

any ForNode::GetStartValueNode() const
{
	return  start_value_node;
}

any ForNode::GetEndValueNode() const
{
	return end_value_node;
}

any ForNode::GetStep() const
{
	return step;
}

any ForNode::GetBodyNode() const
{
	return body_node;
}

Position ForNode::GetStart() const
{
	return start;
}

Position ForNode::GetEnd() const
{
	return end;
}
