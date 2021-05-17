#include "NamespaceNode.h"

NamespaceNode::NamespaceNode(Token name, any statements, Position start, Position end): Node(start,end),name(name), statements(statements)
{
}

Token NamespaceNode::GetName() const
{
	return name;
}

any NamespaceNode::GetStatements() const
{
	return statements;
}
