#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

using namespace std;

class ForNode: public Node
{
	public:
		ForNode(Token var_token_name, any start, any end, any body, any step, Position startPos, Position endPos);
		Token GetVarTokenName() const;
		any GetStartValueNode() const;
		any GetEndValueNode() const;
		any GetStep() const;
		any GetBodyNode() const;
	private:
		Token var_token_name;
		any start_value_node;
		any end_value_node;
		any step;
		any body_node;
};

