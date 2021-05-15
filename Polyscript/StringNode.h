#pragma once
#include "Node.h"
#include "Token.h"


class StringNode : public Node {
	public:
		StringNode(Token token);
		Token GetToken() const;
		friend std::ostream& operator<<(std::ostream& lhs, const StringNode& token);
	private:
		Token token;
};

