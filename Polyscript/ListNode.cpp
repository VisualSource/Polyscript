#include "ListNode.h"

ListNode::ListNode(vector<any> elements, Position start, Position end): Node(start,end), elements(elements)
{
}

vector<any> ListNode::GetElements() const
{
	return elements;
}
