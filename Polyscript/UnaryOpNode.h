#pragma once
#include <any>
#include "Token.h"
#include "Node.h"

class UnaryOpNode: public Node {
	public:
		UnaryOpNode(Token op_tok, std::any node, Position start, Position end);
		Token GetToken() const;
		std::any GetNode() const;
		friend std::ostream& operator<<(std::ostream& lhs, const UnaryOpNode& token);
	private:
		Token op_tok;
		std::any node;
};

