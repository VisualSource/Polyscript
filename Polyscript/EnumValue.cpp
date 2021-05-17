#include "EnumValue.h"
#include "Integer.h"

Integer EnumValue::cast(const EnumValue& value)
{
    return Integer(value.value).SetContext(value.GetContext()).SetPostion(value.GetStart(),value.GetEnd());
}

EnumValue::EnumValue(string owner, string key, int value): owner(owner), key(key), value(value)
{
}

EnumValue& EnumValue::SetContext(Context* context)
{
    this->context = context;
    return *this;
}

EnumValue& EnumValue::SetPostion(Position start, Position end)
{
    this->start = start;
    this->end = end;
    return *this;
}

Integer EnumValue::operator==(const EnumValue& rhs) const
{
    return Integer((int)(owner == rhs.owner && key == rhs.key)).SetContext(context).PrintBool(true);
}

Integer EnumValue::operator!=(const EnumValue& rhs) const
{
    return Integer((int)!(owner == rhs.owner && key == rhs.key)).SetContext(context).PrintBool(true);
}

std::ostream& operator<<(std::ostream& lhs, const EnumValue& other)
{

    lhs << "<\x1B[36menum\033[0m " + other.owner << "> " << "\x1B[36m" << other.key << "\033[0m";
    return lhs;
}
