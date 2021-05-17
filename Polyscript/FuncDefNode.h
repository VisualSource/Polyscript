#pragma once
#include <any>
#include <vector>
#include <optional>
#include "Token.h"
#include "Node.h"

using namespace std;

class FuncDefNode: public Node
{
	public:
		FuncDefNode(optional<Token> var_name_token, const vector<Token>& arg_tokens, any body, Position start, Position end);
		any GetBody() const;
		optional<Token> GetName() const;
		vector<Token> GetArgs() const;
	private:
		any body_node;
		optional<Token> var_name;
		vector<Token> arg_names;
};

