#include "ForNode.h"
#include "NodeUtils.h"

using namespace std;

/*
	FOR expr TO expr (STEP expr)? { expr }
*/
ForNode::ForNode(Token var_token_name, any start, any end, any body, any step, Position startPos, Position endPos): Node(startPos,endPos), var_token_name(var_token_name), start_value_node(start), end_value_node(end), body_node(body), step(step)
{

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

