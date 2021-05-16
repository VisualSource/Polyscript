#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

using namespace std;

class VarReasignNode : public Node {
	public:
		VarReasignNode(Token ident, Token op, any expr, Position start, Position end);
		Token GetIdentifer() const;
		Token GetOperationToken() const;
		any GetExpr() const;
	private:
		Token identifer;
		Token opeartor;
		any expr;
};

