#include "PathAccessNode.h"

PathAccessNode::PathAccessNode(const vector<Token>& path, Position start, Position end)
{
	for (auto item : path) {
		this->path.push_back(item.GetValue().value());
	}
	SetStart(start);
	SetEnd(end);
}

vector<string> PathAccessNode::GetPath() const
{
	return path;
}
