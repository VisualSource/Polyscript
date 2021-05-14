#include "Float.h"
#include <cmath>
#include "Integer.h"
#include "RuntimeError.h"
#include "Context.h"

Float Float::cast(const Integer& value)
{
	return Float((int)value.GetValue()).set_context(value.GetContext());
}

Float::Float(double value): value(value) {
}

void Float::set_pos(optional<Position> start, optional<Position> end) {
	this->start = start;
	this->end = end;
}

Float& Float::set_context(Context* context)
{
	this->context = context;
	return *this;
}

Float Float::power(const Float& other)
{
	return Float(std::pow(value,other.value)).set_context(context);
}

double Float::GetValue() const {
	return value;
}

optional<Position> Float::GetStart() const {
	return start;
}

optional<Position> Float::GetEnd() const {
	return end;
}

Context* Float::GetContext() const
{
	return context;
}

Integer Float::operator==(const Float& rhs) const
{
	return Integer((int)(value == rhs.value)).set_context(context);
}

Integer Float::operator!=(const Float& rhs) const
{
	return Integer((int)(value != rhs.value)).set_context(context);
}

bool operator<(const Float& lhs, const Float& rhs)
{
	return lhs.value < rhs.value;
}

bool operator>(const Float& lhs, const Float& rhs)
{
	return lhs.value > rhs.value;
}

bool operator<=(const Float& lhs, const Float& rhs)
{
	return lhs.value <= rhs.value;
}

bool operator>=(const Float& lhs, const Float& rhs)
{
	return lhs.value >= rhs.value;
}

std::ostream& operator<<(std::ostream& lhs, const Float& other) {
	lhs << "\x1B[92m" << other.value << "\033[0m";
	return lhs;
}

Float operator+(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value + rhs.value).set_context(lhs.context);
}

Float operator-(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value - rhs.value).set_context(lhs.context);
}

Float operator*(const Float& lhs, const Float& rhs)
{
	return Float(lhs.value * rhs.value).set_context(lhs.context);
}

Float operator/(const Float& lhs, const Float& rhs)
{
	if (rhs.value == 0) {
		throw RuntimeError("Division by zero", rhs.context, rhs.GetStart().value(), rhs.GetEnd().value());
	}
	return Float(lhs.value / rhs.value).set_context(lhs.context);
}
