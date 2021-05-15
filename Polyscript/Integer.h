#pragma once
#include "Variable.h"

using namespace std;

class Float;
class String;

class Integer: public Variable {
	public:
		static Integer cast(const Float& value);
		static Integer cast(const String& value);
		Integer(int value);
		Integer power(const Integer& other);
		int GetValue() const;
		virtual Integer& SetContext(Context* context) override;
		virtual Integer& SetPostion(Position start, Position end) override;
		bool isTrue() const { return value == 1 ? true : false; };
		Integer& PrintBool(bool printAsbool = false);
		friend std::ostream& operator<<(std::ostream& lhs, const Integer& other);
		Integer operator==(const Integer& rhs) const;
		Integer operator!=(const Integer& rhs) const;
		Integer operator&&(const Integer& rhs) const;
		Integer operator||(const Integer& rhs) const;
		Integer operator!() const;
		friend Integer operator<(const Integer& lhs, const Integer& rhs);
		friend Integer operator>(const Integer& lhs, const Integer& rhs);
		friend Integer operator<=(const Integer& lhs, const Integer& rhs);
		friend Integer operator>=(const Integer& lhs, const Integer& rhs);
		friend Integer operator+(const Integer& lhs, const Integer& rhs);
		friend Integer operator-(const Integer& lhs, const Integer& rhs);
		friend Integer operator*(const Integer& lhs, const Integer& rhs);
		friend Integer operator/(const Integer& lhs, const Integer& rhs);
	private:
		int value;
		bool printAsBool = false;
};

