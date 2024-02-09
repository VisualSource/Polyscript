#pragma once
#include "../Object.hpp"

namespace jit
{
    class Number : public Object
    {
    private:
        double value;
        bool isBool;

    public:
        Number(double value) : Object(2), value(value), isBool(false) {}
        Number(bool value) : Object(2), value(value ? 1 : 0), isBool(true) {}
        Number() : Object(2), value(0.0) {}
        inline bool isBoolean() const { return isBool; }
        inline double getValue() const { return value; }
        inline bool asBool() const { return (bool)value; }
        friend Number operator+(Number &lhs, const Number &rhs);
        friend Number operator-(Number &lhs, const Number &rhs);
        friend Number operator*(Number &lhs, const Number &rhs);
        friend Number operator/(Number &lhs, const Number &rhs);
        friend bool operator<(const Number &lhs, const Number &rhs);
        inline friend bool operator>(const Number &lhs, const Number &rhs) { return rhs < lhs; }
        inline friend bool operator<=(const Number &lhs, const Number &rhs) { return !(lhs > rhs); }
        inline friend bool operator>=(const Number &lhs, const Number &rhs) { return !(lhs < rhs); }
        friend bool operator==(const Number &lhs, const Number &rhs);
        inline friend bool operator!=(const Number &lhs, const Number &rhs) { return !(lhs == rhs); }

        void print(std::ostream &where) const override;
    };
}