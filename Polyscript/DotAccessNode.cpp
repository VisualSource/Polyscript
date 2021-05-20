#include "DotAccessNode.h"

DotAccessNode::DotAccessNode(const std::vector<std::string>& accessPath, Position start, Position end): Node(start,end), accessPath(accessPath)
{
}


std::vector<std::string> DotAccessNode::GetAccessPath() const
{
	return accessPath;
}
