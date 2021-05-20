#include "Null.h"

Null::Null()
{
}

Null& Null::SetContext(Context* context)
{
	this->context = context;
	return *this;
}

Null& Null::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

std::ostream& operator<<(std::ostream& lhs, const Null& other)
{
	lhs << "\x1B[90mnull\033[0m" << std::endl;
	return lhs;
}
