#pragma once
#include <vector>
#include <any>
#include <optional>
#include "Node.h"

struct IfCases {
	std::any condition;
	std::any expr;
};

class IfNode: public Node
{
	public:
		IfNode(const std::vector<IfCases>& statments, std::optional<std::any> else_statment, Position start, Position end);
		const std::vector<IfCases>& GetIfStatments() const { return ifStatements; };
		std::any GetElseStatment() const { return else_statment; };
	private:
		std::vector<IfCases> ifStatements;
		std::any else_statment;
};

