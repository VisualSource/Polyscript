#pragma once
#include <string>
#include "Position.h"
#include "Token.h"

using namespace std;

class VarAccessNode {
	public:
		VarAccessNode(Token var_token);
		Token GetToken() const;
		Position GetStart() const;
		Position GetEnd() const;
		friend std::ostream& operator<<(std::ostream& lhs, const VarAccessNode& token);
	private:
		Token var_token;
		Position start;
		Position end;
};

