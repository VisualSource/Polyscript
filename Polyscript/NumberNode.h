#pragma once
#include "Token.h"
#include "Position.h"

class NumberNode {
	public:
		NumberNode(Token token);
		Token GetToken() const;
		Position GetStart() const { return start; };
		Position GetEnd() const { return end; };
		friend std::ostream& operator<<(std::ostream& lhs, const NumberNode& token);
	private:
		Token token;
		Position start;
		Position end;
};

