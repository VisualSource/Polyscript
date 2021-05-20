#pragma once
#include "PolyscriptError.h"
class InvalidSyntaxError: public PolyscriptError {
	public:
		InvalidSyntaxError(std::string details, Position start, Position end);
};
 
