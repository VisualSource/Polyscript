#include "String.h"
#include "Integer.h"
#include "Float.h"

String String::cast(const Float& value)
{
    return String(std::to_string(value.GetValue())).SetContext(value.GetContext());
}

String String::cast(const Integer& value)
{
    return String(to_string(value.GetValue())).SetContext(value.GetContext());
}

String::String(string value): value(value)
{
}

String& String::SetContext(Context* context)
{
    this->context = context;
    return *this;
}

String& String::SetPostion(Position start, Position end)
{
    this->start = start;
    this->end = end;
    return *this;
}

string String::GetValue() const
{
    return value;
}

bool String::IsTrue() const
{
    return value.length() > 0;
}

Integer String::operator==(const String& rhs) const
{
    return Integer((int)(this->value == rhs.value)).SetContext(context).PrintBool(true);
}

Integer String::operator!=(const String& rhs) const
{
    return Integer((int)(this->value != rhs.value)).SetContext(context).PrintBool(true);
}

ostream& operator<<(ostream& lhs, const String& rhs)
{
    lhs << "\x1B[92m \"" << rhs.value << "\"\033[0m";
    return lhs;
}

String operator+(const String& lhs, const String& rhs)
{
    return String(lhs.value + rhs.value).SetContext(lhs.context);
}
