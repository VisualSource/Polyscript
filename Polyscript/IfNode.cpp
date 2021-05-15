#include "IfNode.h"
#include "NodeUtils.h"


IfNode::IfNode(const vector<IfCases>& statments, optional<any> else_statment): ifStatements(statments)
{
	
	if (statments.size() > 0) {
		this->SetStart(NodeUtils::GetStartFromNode(statments.at(0).condition));
		this->SetEnd(NodeUtils::GetEndFromNode(statments.at(statments.size() - 1 ).expr));
	}


	if (else_statment.has_value()) {
		this->else_statment = else_statment.value();
		this->SetEnd(NodeUtils::GetEndFromNode(else_statment.value()));
	}

}
