#pragma once
#include <string>
#include "Variable.h"

class Integer;
class Float;

class String : public Variable {
	public:
		static String  cast(const Float& value);
		static String  cast(const Integer& value);
		String(std::string value);
		virtual String& SetContext(Context* context) override;
		virtual String& SetPostion(Position start, Position end) override;
		std::string GetValue() const;
		bool IsTrue() const;
		friend std::ostream& operator<<(std::ostream& lhs, const String& rhs);
		Integer operator==(const String& rhs) const;
		Integer operator!=(const String& rhs) const;
		friend String operator+(const String& lhs, const String& rhs);
	private:
		std::string value;
};

