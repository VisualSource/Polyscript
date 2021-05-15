#include "FuncCallNode.h"
#include "NodeUtils.h"

FuncCallNode::FuncCallNode(any call_node, vector<any> arg_nodes) : call_node(call_node), arg_nodes(arg_nodes)
{
	SetStart(NodeUtils::GetStartFromNode(call_node)); // TODO get Start from call node;

	//TODO get postion from any
	if (arg_nodes.size() > 0) {
		SetEnd(NodeUtils::GetEndFromNode(arg_nodes.at(arg_nodes.size() - 1)));
	}
	else {
		SetEnd(NodeUtils::GetEndFromNode(call_node));
	}
}

any FuncCallNode::GetCallNode() const
{
	return call_node;
}

vector<any> FuncCallNode::GetArgNodes() const
{
	return arg_nodes;
}
