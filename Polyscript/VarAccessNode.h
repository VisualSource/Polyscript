#pragma once
#include <string>
#include "Token.h"
#include "Node.h"

using namespace std;

class VarAccessNode: public Node {
	public:
		VarAccessNode(Token var_token, Position start, Position end);
		Token GetToken() const;
		friend std::ostream& operator<<(std::ostream& lhs, const VarAccessNode& token);
	private:
		Token var_token;
};

