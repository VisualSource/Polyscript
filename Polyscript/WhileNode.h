#pragma once
#include <any>
#include "Node.h"

using namespace std;

class WhileNode: public Node
{
	public:
		WhileNode(any conditon, any body, Position start, Position end);
		any GetConditionNode() const;
		any GetBodyNode() const;
	private:
		any conditon_node;
		any body_node;
};

