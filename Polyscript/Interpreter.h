#pragma once
#include <any>
#include <ostream>
#include "NodeUtils.h"
#include "Context.h"
#include "Nodes.h"


class FunctionReturn {
	public:
		FunctionReturn(any value);
		any GetValue() const;
	private:
		any value;
};

class BreakEvent {};

class ContinueEvent {};


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
		any visit_VarReasignNode(const VarReasignNode& node, Context* context);
		any visit_PathAccessNode(const PathAccessNode& node, Context* context);
		any visit_IfNode(const IfNode& node, Context* context);
		any visit_ForNode(const ForNode& node, Context* context);
		any visit_WhileNode(const WhileNode& node, Context* context);
		any visit_FuncDefNode(const FuncDefNode& node, Context* context);
		any visit_FuncCallNode(const FuncCallNode& node, Context* context);
		any visit_StringNode(const StringNode& node, Context* context);
		any visit_ListNode(const ListNode& node, Context* context);
		any visit_ListAccessNode(const ListAccessNode& node, Context* context);
		any visit_ReturnNode(const ReturnNode& node, Context* context);
		any visit_EnumDefNode(const EnumNode& node, Context* context);
		any visit_NamespaceNode(const NamespaceNode& node, Context* context);
		any visit_ImportNode(const ImportNode& node, Context* context);
		any visit_MatchNode(const MatchNode& node, Context* context);
};


