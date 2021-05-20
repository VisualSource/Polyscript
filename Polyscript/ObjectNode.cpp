#include "ObjectNode.h"

ObjectNode::ObjectNode(std::vector<ObjectNodeValue> values, Position start, Position end): Node(start,end), values(values)
{
}

std::vector<ObjectNodeValue> ObjectNode::GetValues() const
{
	return values;
}
