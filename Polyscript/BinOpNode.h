#pragma once
#include <any>
#include "Token.h"
#include "Node.h"

class BinOpNode: public Node {
	public:
		BinOpNode(std::any left_node, Token op_token, std::any right_node, Position start, Position end);
		Token GetOpToken() const;
		std::any GetRightNode() const;
		std::any GetLeftNode() const;
		friend std::ostream& operator<<(std::ostream& lhs, const BinOpNode& token);
	private:
		std::any left_node;
		std::any right_node;
		Token op_token;
};

