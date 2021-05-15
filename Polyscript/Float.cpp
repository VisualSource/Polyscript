#include "Float.h"
#include <cmath>
#include "Integer.h"
#include "RuntimeError.h"
#include "Context.h"
#include "String.h"

Float Float::cast(const Integer& value)
{
	return Float((int)value.GetValue()).SetContext(value.GetContext()).SetPostion(value.GetStart(), value.GetEnd());;
}

Float Float::cast(const String& value)
{
	return Float(stod(value.GetValue())).SetContext(value.GetContext()).SetPostion(value.GetStart(),value.GetEnd());
}

Float::Float(double value): value(value) {
}


Float Float::power(const Float& other)
{
	return Float(std::pow(value,other.value)).SetContext(context);
}

double Float::GetValue() const {
	return value;
}

Float& Float::SetContext(Context* ctx)
{
	this->context = ctx;
	return *this;
}

Float& Float::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

Integer Float::operator==(const Float& rhs) const
{
	return Integer((int)(value == rhs.value)).SetContext(context).PrintBool(true);
}

Integer Float::operator!=(const Float& rhs) const
{
	return Integer((int)(value != rhs.value)).SetContext(context).PrintBool(true);
}

Integer operator<(const Float& lhs, const Float& rhs)
{
	return Integer((int)(lhs.value < rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator>(const Float& lhs, const Float& rhs)
{
	return Integer((int)(lhs.value > rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator<=(const Float& lhs, const Float& rhs)
{
	return Integer((int)(lhs.value <= rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator>=(const Float& lhs, const Float& rhs)
{
	return Integer((int)(lhs.value >= rhs.value)).SetContext(lhs.context).PrintBool(true);
}

std::ostream& operator<<(std::ostream& lhs, const Float& other) {
	lhs << "\x1B[93m" << other.value << "\033[0m";
	return lhs;
}

Float operator+(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value + rhs.value).SetContext(lhs.context);
}

Float operator-(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value - rhs.value).SetContext(lhs.context);
}

Float operator*(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value * rhs.value).SetContext(lhs.context);
}

Float operator/(const Float& lhs, const Float& rhs)
{
	if (rhs.value == 0) {
		throw RuntimeError("Division by zero", rhs.context, rhs.GetStart(), rhs.GetEnd());
	}
	return Float(lhs.value / rhs.value).SetContext(lhs.context);
}
