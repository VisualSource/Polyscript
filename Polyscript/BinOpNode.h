#pragma once
#include <any>
#include "Token.h"
#include "Node.h"

using namespace std;

class BinOpNode: public Node {
	public:
		BinOpNode(any left_node, Token op_token, any right_node, Position start, Position end);
		Token GetOpToken() const;
		any GetRightNode() const;
		any GetLeftNode() const;
		friend std::ostream& operator<<(std::ostream& lhs, const BinOpNode& token);
	private:
		any left_node;
		any right_node;
		Token op_token;
};

