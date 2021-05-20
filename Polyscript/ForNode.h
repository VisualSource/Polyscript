#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

class ForNode: public Node
{
	public:
		ForNode(Token var_token_name, std::any start, std::any end, std::any body, std::any step, Position startPos, Position endPos);
		Token GetVarTokenName() const;
		std::any GetStartValueNode() const;
		std::any GetEndValueNode() const;
		std::any GetStep() const;
		std::any GetBodyNode() const;
	private:
		Token var_token_name;
		std::any start_value_node;
		std::any end_value_node;
		std::any step;
		std::any body_node;
};

