#pragma once
#include <any>
#include "Token.h"
#include "Position.h"
class UnaryOpNode {
	public:
		UnaryOpNode(Token op_tok, any node);
		Token GetToken() const;
		any GetNode() const;
		Position GetEnd() const { return end; };
		Position GetStart() const { return start; };
		friend std::ostream& operator<<(std::ostream& lhs, const UnaryOpNode& token);
	private:
		Position start;
		Position end;
		Token op_tok;
		any node;
};

