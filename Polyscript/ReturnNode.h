#pragma once
#include "Node.h"
#include <any>

class ReturnNode : public Node {
	public:
		ReturnNode(std::any return_value, Position start, Position end);
		std::any GetReturnValue() const;
	private:
		std::any return_value;
};

