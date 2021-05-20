#pragma once
#include "PolyscriptError.h"
class ExpectedCharError : public PolyscriptError {
	public:
		ExpectedCharError(std::string details, Position start, Position end);
};

