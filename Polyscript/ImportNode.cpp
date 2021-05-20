#include "ImportNode.h"

using namespace std;

ImportNode::ImportNode(string path, Position start, Position end): Node(start,end), path(path)
{
}

string ImportNode::GetPath() const
{
	return path;
}
