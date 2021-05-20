#include "IfNode.h"
#include "NodeUtils.h"

using namespace std;


IfNode::IfNode(const vector<IfCases>& statments, optional<any> else_statment, Position start, Position end): Node(start,end), ifStatements(statments)
{
	if (else_statment.has_value()) {
		this->else_statment = else_statment.value();
	}
}
