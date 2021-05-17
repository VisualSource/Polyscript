#include "PathAccessNode.h"

PathAccessNode::PathAccessNode(const vector<Token>& path, Position start, Position end) : Node(start,end)
{
	for (auto item : path) {
		this->path.push_back(item.GetValue().value());
	}
}

vector<string> PathAccessNode::GetPath() const
{
	return path;
}
