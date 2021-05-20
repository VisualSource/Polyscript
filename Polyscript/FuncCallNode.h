#pragma once
#include "Node.h"
#include <any>
#include <vector>

class FuncCallNode: public Node
{
	public: 
		FuncCallNode(std::any call_node, std::vector<std::any> arg_nodes, Position start, Position end);
		std::any GetCallNode() const;
		std::vector<std::any> GetArgNodes() const;
	private:
		std::any call_node;
		std::vector<std::any> arg_nodes;
};

