#pragma once
#include <ostream>
#include "Position.h"

class Context;

class Variable
{
	public:
		Position GetStart() const;
		Position GetEnd() const;
		Context* GetContext() const;
		virtual Variable& SetContext(Context* context) = 0;
		virtual Variable& SetPostion(Position start, Position end) = 0;
		friend std::ostream& operator<<(std::ostream& lhs, const Variable& rhs);
	protected:
		Context* context = nullptr;
		Position start;
		Position end;
};

