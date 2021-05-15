#include "ListNode.h"

ListNode::ListNode(vector<any> elements, Position start, Position end): elements(elements)
{
	SetStart(start);
	SetEnd(end);
}

vector<any> ListNode::GetElements() const
{
	return elements;
}
