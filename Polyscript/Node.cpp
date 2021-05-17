#include "Node.h"

Node::Node(Position start, Position end): start(start), end(end)
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

