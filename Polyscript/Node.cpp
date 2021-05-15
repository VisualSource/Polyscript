#include "Node.h"

Node::Node()
{
}

Position Node::GetStart() const
{
    return start;
}

Position Node::GetEnd() const
{
    return end;
}

void Node::SetStart(Position pos)
{
    start = pos;
}

void Node::SetEnd(Position pos)
{
    end = pos;
}
