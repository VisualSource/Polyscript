#include "Integer.h"
#include <cmath>
#include "Float.h"
#include "RuntimeError.h"
#include "Context.h"

Integer Integer::cast(const Float& value)
{
	return Integer((int)value.GetValue()).set_context(value.GetContext());
}

Integer::Integer(int value): value(value) {
}

void Integer::set_pos(optional<Position> start, optional<Position> end) {
	this->start = start;
	this->end = end;
}

Integer& Integer::set_context(Context* context)
{
	this->context = context;
	return *this;
}

Integer Integer::power(const Integer& other){
	return Integer((int)std::pow(value,other.value)).set_context(context);
}

int Integer::GetValue() const {
	return value;
}

optional<Position> Integer::GetStart() const {
	return start;
}

optional<Position> Integer::GetEnd() const {
	return end;
}

Context* Integer::GetContext() const
{
	return context;
}

Integer Integer::operator==(const Integer& rhs) const
{
	return Integer((int)(value == rhs.value)).set_context(context);
}

Integer Integer::operator!=(const Integer& rhs) const
{
	return Integer((int)(value != rhs.value)).set_context(context);
}

Integer Integer::operator&&(const Integer& rhs) const
{
	return Integer((int)((bool)value && (bool)rhs.value)).set_context(context);
}

Integer Integer::operator||(const Integer& rhs) const
{
	return Integer((int)((bool)value || (bool)rhs.value)).set_context(context);
}

Integer Integer::operator!() const
{
	if(value == 0){
		return Integer(1).set_context(context);
	}
	return Integer(0).set_context(context);
}

std::ostream& operator<<(std::ostream& lhs, const Integer& other) {

	lhs << "\x1B[93m" << other.value << "\033[0m"; 
	return lhs;
}

Integer operator<(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value < rhs.value)).set_context(lhs.context);
}

Integer operator>(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value > rhs.value)).set_context(lhs.context);
}

Integer operator<=(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value <= rhs.value)).set_context(lhs.context);
}

Integer operator>=(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value >= rhs.value)).set_context(lhs.context);
}

Integer operator+(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value + rhs.value).set_context(lhs.context);
}

Integer operator-(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value - rhs.value).set_context(lhs.context);
}

Integer operator*(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value * rhs.value).set_context(lhs.context);
}

Integer operator/(const Integer& lhs, const Integer& rhs)
{
	if (rhs.value == 0) {
		throw RuntimeError("Division by zero",rhs.context, rhs.GetStart().value(), rhs.GetEnd().value());
	}
	return Integer(lhs.value / rhs.value).set_context(lhs.context);
}

