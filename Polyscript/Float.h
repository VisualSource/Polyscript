#pragma once
#include <optional>
#include <ostream>
#include "Position.h"

using namespace std;

class Integer;
class Context;

class Float {
	public:
		static Float cast(const Integer& value);
		Float(double value);
		void set_pos(optional<Position> start, optional<Position> end);
		Float& set_context(Context* context);
		Float power(const Float& other);
		double GetValue() const;
		optional<Position> GetStart() const;
		optional<Position> GetEnd() const;
		Context* GetContext() const;
		Integer operator==(const Float& rhs) const;
		Integer operator!=(const Float& rhs) const;
		friend bool operator<(const Float& lhs, const Float& rhs);
		friend bool operator>(const Float& lhs, const Float& rhs);
		friend bool operator<=(const Float& lhs, const Float& rhs);
		friend bool operator>=(const Float& lhs, const Float& rhs);
		friend std::ostream& operator<<(std::ostream& lhs, const Float& other);
		friend Float operator+(const Float& lhs, const Float& rhs);
		friend Float operator-(const Float& lhs, const Float& rhs);
		friend Float operator*(const Float& lhs, const Float& rhs);
		friend Float operator/(const Float& lhs, const Float& rhs);
	private:
		Context* context = nullptr;
		double value;
		optional<Position> start = nullopt;
		optional<Position> end = nullopt;
};

