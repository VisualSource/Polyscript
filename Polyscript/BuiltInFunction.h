#pragma once
#include "BaseFunction.h"

class BuiltInFunction : public BaseFunction {
	public:
		BuiltInFunction(std::string name, std::vector<std::string> args_names);
		virtual std::any exceute(std::vector<std::any> args) override;
	protected:
		std::vector<std::string> args_names;
		
};

