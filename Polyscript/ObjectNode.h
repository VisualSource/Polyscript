#pragma once
#include <any>
#include <vector>
#include "Node.h"
#include "Token.h"

struct ObjectNodeValue {
	Token name;
	std::any expr;
};

class ObjectNode : public Node {
	public:
		ObjectNode(std::vector<ObjectNodeValue> values,Position start, Position end);
		std::vector<ObjectNodeValue> GetValues() const;
	private:
		std::vector<ObjectNodeValue> values;
};

