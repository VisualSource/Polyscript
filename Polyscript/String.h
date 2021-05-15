#pragma once
#include <string>
#include "Variable.h"

using namespace std;

class Integer;
class Float;

class String : public Variable {
	public:
		static String  cast(const Float& value);
		static String  cast(const Integer& value);
		String(string value);
		virtual String& SetContext(Context* context) override;
		virtual String& SetPostion(Position start, Position end) override;
		string GetValue() const;
		bool IsTrue() const;
		friend ostream& operator<<(ostream& lhs, const String& rhs);
		Integer operator==(const String& rhs) const;
		Integer operator!=(const String& rhs) const;
		friend String operator+(const String& lhs, const String& rhs);
	private:
		string value;
};

