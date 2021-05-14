#pragma once
#include <any>
#include "Token.h"
#include "Position.h"

using namespace std;

class BinOpNode {
	public:
		BinOpNode(any left_node, Token op_token, any right_node);
		Token GetOpToken() const;
		any GetRightNode() const;
		any GetLeftNode() const;
		Position GetStart() const { return start; };
		Position GetEnd() const { return end; };
		friend std::ostream& operator<<(std::ostream& lhs, const BinOpNode& token);
	private:
		Position start;
		Position end;
		any left_node;
		any right_node;
		Token op_token;
};

