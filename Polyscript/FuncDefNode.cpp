#include "FuncDefNode.h"
#include "NodeUtils.h"

FuncDefNode::FuncDefNode(optional<Token> var_name_token, const vector<Token>& arg_tokens, any body, Position start, Position end): Node(start,end), var_name(var_name_token), arg_names(arg_tokens), body_node(body)
{
}

any FuncDefNode::GetBody() const
{
	return body_node;
}

optional<Token> FuncDefNode::GetName() const
{
	return var_name;
}

vector<Token> FuncDefNode::GetArgs() const
{
	return arg_names;
}