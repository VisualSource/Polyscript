#pragma once
#include "PolyscriptError.h"
class ExpectedCharError : public PolyscriptError {
	public:
		ExpectedCharError(string details, Position start, Position end);
};

