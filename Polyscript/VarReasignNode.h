#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

class VarReasignNode : public Node {
	public:
		VarReasignNode(Token ident, Token op, std::any expr, Position start, Position end);
		Token GetIdentifer() const;
		Token GetOperationToken() const;
		std::any GetExpr() const;
	private:
		Token identifer;
		Token opeartor;
		std::any expr;
};

