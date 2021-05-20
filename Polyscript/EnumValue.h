#pragma once
#include "Variable.h"

class Integer;

class EnumValue : public Variable {
	public:
		static Integer cast(const EnumValue& value);
		EnumValue(std::string owner, std::string key, int value);
		virtual EnumValue& SetContext(Context* context) override;
		virtual EnumValue& SetPostion(Position start, Position end) override;
		friend std::ostream& operator<<(std::ostream& lhs, const EnumValue& other);
		Integer operator==(const EnumValue& rhs) const;
		Integer operator!=(const EnumValue& rhs) const;
	private:
		std::string owner;
		std::string key;
		int value;
};

