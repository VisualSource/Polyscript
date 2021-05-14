#pragma once
#include <any>
#include "Position.h"

using namespace std;

class WhileNode
{
	public:
		WhileNode(any conditon, any body);
		Position GetStart() const;
		Position GetEnd() const;
		any GetConditionNode() const;
		any GetBodyNode() const;
	private:
		Position start;
		Position end;
		any conditon_node;
		any body_node;
};

