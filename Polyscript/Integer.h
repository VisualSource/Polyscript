#pragma once
#include <optional>
#include <ostream>
#include "Position.h"

using namespace std;

class Float;
class Context;


class Integer {
	public:
		static Integer cast(const Float& value);
		Integer(int value);
		void set_pos(optional<Position> start, optional<Position> end);
		Integer& set_context(Context* context);
		Integer power(const Integer& other);
		int GetValue() const;
		optional<Position> GetStart() const;
		optional<Position> GetEnd() const;
		Context* GetContext() const;
		bool isTrue() const { return value == 1 ? true : false; };
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
		Context* context = nullptr;
		optional<Position> start = nullopt;
		optional<Position> end = nullopt;
};

