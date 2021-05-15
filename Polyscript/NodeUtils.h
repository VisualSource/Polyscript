#pragma once
#include <ostream>
#include <any>


using namespace std;

class Position;

namespace NodeUtils {
	ostream& printNode(std::ostream& lhs, const any& node);
	Position GetStartFromNode(const any& node);
	Position GetEndFromNode(const any& node);
	bool isBinOpNode(const any& node);
	bool isNumberNode(const any& node);
	bool isUnaryOpNode(const any& node);
	bool isVarAccessNode(const any& node);
	bool isAssignNode(const any& node);
	bool isIfNode(const any& node);
	bool isWhileNode(const any& node);
	bool isForNode(const any& node);
	bool isFuncDefNode(const any& node);
	bool isFuncCallNode(const any& node);
	bool isStringNode(const any& node);
	bool isListNode(const any& node);
	bool isListAccessNode(const any& node);
}
