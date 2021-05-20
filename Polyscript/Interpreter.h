#pragma once
#include <any>
#include <ostream>
#include "NodeUtils.h"
#include "Context.h"
#include "Nodes.h"


class FunctionReturn {
	public:
		FunctionReturn(std::any value);
		std::any GetValue() const;
	private:
		std::any value;
};

class BreakEvent {};

class ContinueEvent {};


class Interpreter {
	public:
		Interpreter();
		std::any visit(const std::any& node, Context* context);
	private:
		std::any visit_NumberNode(const NumberNode& node, Context* context);
		std::any visit_BinOpNode(const BinOpNode& node, Context* context);
		std::any visit_UnaryOpNode(const UnaryOpNode& node, Context* context);
		std::any visit_VarAccessNode(const VarAccessNode& node, Context* context);
		std::any visit_VarAssignNode(const VarAssignNode& node, Context* context);
		std::any visit_VarReasignNode(const VarReasignNode& node, Context* context);
		std::any visit_PathAccessNode(const PathAccessNode& node, Context* context);
		std::any visit_IfNode(const IfNode& node, Context* context);
		std::any visit_ForNode(const ForNode& node, Context* context);
		std::any visit_WhileNode(const WhileNode& node, Context* context);
		std::any visit_FuncDefNode(const FuncDefNode& node, Context* context);
		std::any visit_FuncCallNode(const FuncCallNode& node, Context* context);
		std::any visit_StringNode(const StringNode& node, Context* context);
		std::any visit_ListNode(const ListNode& node, Context* context);
		std::any visit_ListAccessNode(const ListAccessNode& node, Context* context);
		std::any visit_ReturnNode(const ReturnNode& node, Context* context);
		std::any visit_EnumDefNode(const EnumNode& node, Context* context);
		std::any visit_NamespaceNode(const NamespaceNode& node, Context* context);
		std::any visit_ImportNode(const ImportNode& node, Context* context);
		std::any visit_MatchNode(const MatchNode& node, Context* context);
		std::any visit_ObjectNode(const ObjectNode& node, Context* context);
		std::any visit_DotAccessNode(const DotAccessNode& node, Context* context);
};


