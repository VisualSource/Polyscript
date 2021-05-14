#pragma once
#include "PolyscriptError.h"
class IllegalCharError : public PolyscriptError {
	public:
		IllegalCharError(string details, Position start, Position end);
};

