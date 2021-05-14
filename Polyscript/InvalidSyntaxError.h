#pragma once
#include "PolyscriptError.h"
class InvalidSyntaxError: public PolyscriptError {
	public:
		InvalidSyntaxError(string details, Position start, Position end);
};
 
