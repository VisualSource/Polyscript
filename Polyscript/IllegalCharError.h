#pragma once
#include "PolyscriptError.h"
class IllegalCharError : public PolyscriptError {
	public:
		IllegalCharError(std::string details, Position start, Position end);
};

