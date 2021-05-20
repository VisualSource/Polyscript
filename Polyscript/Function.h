#pragma once
#include "BaseFunction.h"

class Function: public BaseFunction
{
	public:
		Function(std::any body_node, std::vector<std::string> arg_names, Position start, Position end, std::string name = "<anonymous>");
		virtual std::any exceute(std::vector<std::any> args) override;
	private:
		std::any body_node;
		std::vector<std::string> arg_names;
};

