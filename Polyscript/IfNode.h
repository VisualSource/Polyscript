#pragma once
#include "Position.h"
#include <vector>
#include <any>
#include <optional>

using namespace std;

struct IfCases {
	any condition;
	any expr;
};

class IfNode
{
	public:
		IfNode(const vector<IfCases>& statments, optional<any> else_statment);
		const vector<IfCases>& GetIfStatments() const { return ifStatements; };
		any GetElseStatment() const { return else_statment; };
		Position GetStart() const { return start; };
		Position GetEnd() const { return end; };
	private:
		vector<IfCases> ifStatements;
		any else_statment;
		Position start;
		Position end;
};

