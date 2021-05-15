#include "FuncDefNode.h"
#include "NodeUtils.h"

FuncDefNode::FuncDefNode(optional<Token> var_name_token, const vector<Token>& arg_tokens, any body): var_name(var_name_token), arg_names(arg_tokens), body_node(body)
{
	if (var_name_token.has_value()) {
		SetStart(var_name_token.value().GetStart().value());
	}else if(arg_tokens.size() > 0){
		SetStart(arg_tokens.at(0).GetStart().value());
	}else if(body.has_value()){
		SetStart(NodeUtils::GetStartFromNode(body));
		//TODO have to do a cast here to get start peostion from body;
	}
	else {
		SetStart(Position());
	}

	// TODO create fucntion to get postions from any nodes
	if (body.has_value()) {
		SetEnd(NodeUtils::GetEndFromNode(body));
	}
	else {
		SetEnd(Position());
	}
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