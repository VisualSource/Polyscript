#pragma once
#include "Variable.h"
class Null : public Variable
{
	public:
		Null();
		bool isTrue() const { return false; };
		virtual Null& SetContext(Context* context) override;
		virtual Null& SetPostion(Position start, Position end) override;
		friend std::ostream& operator<<(std::ostream& lhs, const Null& other);
};

