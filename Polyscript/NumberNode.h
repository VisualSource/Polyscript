#pragma once
#include "Token.h"
#include "Node.h"

class NumberNode: public Node {
	public:
		NumberNode(Token token);
		Token GetToken() const;
		friend std::ostream& operator<<(std::ostream& lhs, const NumberNode& token);
	private:
		Token token;
};

