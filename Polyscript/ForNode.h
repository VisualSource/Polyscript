#pragma once
#include <any>
#include "Token.h"
#include "Position.h"

using namespace std;

class ForNode
{
	public:
		ForNode(Token var_token_name, any start, any end, any body, any step);
		Token GetVarTokenName() const;
		any GetStartValueNode() const;
		any GetEndValueNode() const;
		any GetStep() const;
		any GetBodyNode() const;
		Position GetStart() const;
		Position GetEnd() const;
	private:
		Token var_token_name;
		any start_value_node;
		any end_value_node;
		any step;
		any body_node;
		Position start;
		Position end;
};

