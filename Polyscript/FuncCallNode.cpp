#include "FuncCallNode.h"
#include "NodeUtils.h"

FuncCallNode::FuncCallNode(any call_node, vector<any> arg_nodes, Position start, Position end): Node(start,end), call_node(call_node), arg_nodes(arg_nodes)
{
}

any FuncCallNode::GetCallNode() const
{
	return call_node;
}

vector<any> FuncCallNode::GetArgNodes() const
{
	return arg_nodes;
}
