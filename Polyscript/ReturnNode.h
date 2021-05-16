#pragma once
#include "Node.h"
#include <any>

using namespace std;


class ReturnNode : public Node {
	public:
		ReturnNode(any return_value, Position start, Position end);
		any GetReturnValue() const;
	private:
		any return_value;
};

