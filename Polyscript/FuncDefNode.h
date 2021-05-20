#pragma once
#include <any>
#include <vector>
#include <optional>
#include "Token.h"
#include "Node.h"

class FuncDefNode: public Node
{
	public:
		FuncDefNode(std::optional<Token> var_name_token, const std::vector<Token>& arg_tokens, std::any body, Position start, Position end);
		std::any GetBody() const;
		std::optional<Token> GetName() const;
		std::vector<Token> GetArgs() const;
	private:
		std::any body_node;
		std::optional<Token> var_name;
		std::vector<Token> arg_names;
};

