#pragma once
#include <vector>
#include <any>
#include <optional>
#include "Node.h"

using namespace std;

struct IfCases {
	any condition;
	any expr;
};

class IfNode: public Node
{
	public:
		IfNode(const vector<IfCases>& statments, optional<any> else_statment, Position start, Position end);
		const vector<IfCases>& GetIfStatments() const { return ifStatements; };
		any GetElseStatment() const { return else_statment; };
	private:
		vector<IfCases> ifStatements;
		any else_statment;
};

