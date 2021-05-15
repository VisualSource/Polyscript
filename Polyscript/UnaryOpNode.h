#pragma once
#include <any>
#include "Token.h"
#include "Node.h"

class UnaryOpNode: public Node {
	public:
		UnaryOpNode(Token op_tok, any node);
		Token GetToken() const;
		any GetNode() const;
		friend std::ostream& operator<<(std::ostream& lhs, const UnaryOpNode& token);
	private:
		Token op_tok;
		any node;
};

