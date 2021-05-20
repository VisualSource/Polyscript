#include "Variable.h"
#include "Context.h"

Position Variable::GetStart() const
{
    return start;
}

Position Variable::GetEnd() const
{
    return end;
}

Context* Variable::GetContext() const
{
    return context;
}

Variable& Variable::SetContext(Context* context)
{
    this->context = context;
    return *this;
}

Variable& Variable::SetPostion(Position start, Position end)
{
    this->start = start;
    this->end = end;
    return *this;
}

std::ostream& operator<<(std::ostream& lhs, const Variable& rhs)
{
    lhs << "<object>";
    return lhs;
}
