#pragma once
#include <any>
#include "Node.h"

class WhileNode: public Node
{
	public:
		WhileNode(std::any conditon, std::any body, Position start, Position end);
		std::any GetConditionNode() const;
		std::any GetBodyNode() const;
	private:
		std::any conditon_node;
		std::any body_node;
};

