#pragma once
#include <ostream>
#include <any>
#include "BinOpNode.h"
#include "NumberNode.h"
#include "UnaryOpNode.h"
#include "VarAssignNode.h"
#include "VarAccessNode.h"
#include "IfNode.h"

using namespace std;

namespace NodeUtils {
	ostream& printNode(std::ostream& lhs, const any& node);
	bool isBinOpNode(const any& node);
	bool isNumberNode(const any& node);
	bool isUnaryOpNode(const any& node);
	bool isVarAccessNode(const any& node);
	bool isAssignNode(const any& node);
	bool isIfNode(const any& node);
}
