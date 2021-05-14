#pragma once
#include <any>
#include <ostream>
#include "NodeUtils.h"
#include "Context.h"
#include "Nodes.h"


class Interpreter {
	public:
		Interpreter();
		any visit(const any& node, Context* context);
	private:
		any visit_NumberNode(const NumberNode& node, Context* context);
		any visit_BinOpNode(const BinOpNode& node, Context* context);
		any visit_UnaryOpNode(const UnaryOpNode& node, Context* context);
		any visit_VarAccessNode(const VarAccessNode& node, Context* context);
		any visit_VarAssignNode(const VarAssignNode& node, Context* context);
		any visit_IfNode(const IfNode& node, Context* context);
		any visit_ForNode(const ForNode& node, Context* context);
		any visit_WhileNode(const WhileNode& node, Context* context);
};

namespace InterTypes {
	bool isFloat(const any& node);
	bool isInteger(const any& node);
	void print(ostream& rhs, const any& node);
}

