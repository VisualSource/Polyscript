#pragma once
#include "Variable.h"


using namespace std;

class Integer;
class String;

class Float: public Variable {
	public:
		static Float cast(const Integer& value);
		static Float cast(const String& value);
		Float(double value);
		Float power(const Float& other);
		double GetValue() const;
		virtual Float& SetContext(Context* context) override;
		virtual Float& SetPostion(Position start, Position end) override;
		Integer operator==(const Float& rhs) const;
		Integer operator!=(const Float& rhs) const;
		friend Integer operator<(const Float& lhs, const Float& rhs);
		friend Integer operator>(const Float& lhs, const Float& rhs);
		friend Integer operator<=(const Float& lhs, const Float& rhs);
		friend Integer operator>=(const Float& lhs, const Float& rhs);
		friend std::ostream& operator<<(std::ostream& lhs, const Float& other);
		friend Float operator+(const Float& lhs, const Float& rhs);
		friend Float operator-(const Float& lhs, const Float& rhs);
		friend Float operator*(const Float& lhs, const Float& rhs);
		friend Float operator/(const Float& lhs, const Float& rhs);
	private:
		double value;	
};

