#pragma once
#include "Node.h"
#include <any>
#include <vector>

using namespace std;


class FuncCallNode: public Node
{
	public: 
		FuncCallNode(any call_node, vector<any> arg_nodes);
		any GetCallNode() const;
		vector<any> GetArgNodes() const;
	private:
		any call_node;
		vector<any> arg_nodes;
};

