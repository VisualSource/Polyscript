#pragma once
#include "Position.h"
class Node
{
	public:
		Node();
		Position GetStart() const;
		Position GetEnd() const;
	protected:
		void SetStart(Position pos);
		void SetEnd(Position pos);
	private:
		Position start;
		Position end;
};

