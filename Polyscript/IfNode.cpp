#include "IfNode.h"


IfNode::IfNode(const vector<IfCases>& statments, optional<any> else_statment): ifStatements(statments)
{
	if (else_statment.has_value()) {
		this->else_statment = else_statment.value();
	}
	start = Position();
	end = Position();
}
