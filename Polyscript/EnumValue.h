#pragma once
#include "Variable.h"


using namespace std;
class Integer;


class EnumValue : public Variable {
	public:
		static Integer cast(const EnumValue& value);
		EnumValue(string owner, string key, int value);
		virtual EnumValue& SetContext(Context* context) override;
		virtual EnumValue& SetPostion(Position start, Position end) override;
		friend std::ostream& operator<<(std::ostream& lhs, const EnumValue& other);
		Integer operator==(const EnumValue& rhs) const;
		Integer operator!=(const EnumValue& rhs) const;
	private:
		string owner;
		string key;
		int value;
};

