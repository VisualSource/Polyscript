#pragma once
#include "Context.h"
#include "PolyscriptError.h"

class RuntimeError: public PolyscriptError {
	public:
		RuntimeError(string details, Context* context, Position start, Position end);
		friend std::ostream& operator<<(std::ostream& lhs, const RuntimeError& error);
	private:
		string GenerateTraceback();
		Context* context = nullptr;
};

