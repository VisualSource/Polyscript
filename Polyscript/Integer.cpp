#include "Integer.h"
#include <cmath>
#include "Float.h"
#include "RuntimeError.h"
#include "Context.h"
#include "String.h"

Integer Integer::cast(const Float& value)
{
	return Integer((int)value.GetValue()).SetContext(value.GetContext());
}

Integer Integer::cast(const String& value)
{
	return Integer(stoi(value.GetValue()));
}

Integer::Integer(int value): value(value) {
}


Integer Integer::power(const Integer& other){
	return Integer((int)std::pow(value,other.value)).SetContext(context);
}

int Integer::GetValue() const {
	return value;
}

Integer& Integer::SetContext(Context* ctx)
{
	this->context = ctx;
	return *this;
}

Integer& Integer::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

Integer& Integer::PrintBool(bool print)
{
	this->printAsBool = print;
	return *this;
}


Integer Integer::operator==(const Integer& rhs) const
{
	return Integer((int)(value == rhs.value)).SetContext(context).PrintBool(true);
}

Integer Integer::operator!=(const Integer& rhs) const
{
	return Integer((int)(value != rhs.value)).SetContext(context).PrintBool(true);
}

Integer Integer::operator&&(const Integer& rhs) const
{
	return Integer((int)((bool)value && (bool)rhs.value)).SetContext(context).PrintBool(true);
}

Integer Integer::operator||(const Integer& rhs) const
{
	return Integer((int)((bool)value || (bool)rhs.value)).SetContext(context).PrintBool(true);
}

Integer Integer::operator!() const
{
	if(value == 0){
		return Integer(1).SetContext(context).PrintBool(true);
	}
	return Integer(0).SetContext(context).PrintBool(true);
}

std::ostream& operator<<(std::ostream& lhs, const Integer& other) {

	if (other.printAsBool) {
		lhs << "\x1B[93m" << (other.value == 0 ? "false" : "true") << "\033[0m";
		return lhs;
	}
	lhs << "\x1B[93m" << other.value << "\033[0m";
	return lhs;
}

Integer operator<(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value < rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator>(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value > rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator<=(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value <= rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator>=(const Integer& lhs, const Integer& rhs)
{
	return Integer((int)(lhs.value >= rhs.value)).SetContext(lhs.context).PrintBool(true);
}

Integer operator+(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value + rhs.value).SetContext(lhs.context);
}

Integer operator-(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value - rhs.value).SetContext(lhs.context);
}

Integer operator*(const Integer& lhs, const Integer& rhs)
{
	return Integer(lhs.value * rhs.value).SetContext(lhs.context);
}

Integer operator/(const Integer& lhs, const Integer& rhs)
{
	if (rhs.value == 0) {
		throw RuntimeError("Division by zero",rhs.GetContext(), rhs.GetStart(), rhs.GetEnd());
	}
	return Integer(lhs.value / rhs.value).SetContext(lhs.GetContext());
}

