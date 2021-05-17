#pragma once
#include "Position.h"
class Node
{
	public:
		Node(Position start, Position end);
		Position GetStart() const;
		Position GetEnd() const;
	private:
		Position start;
		Position end;
};

