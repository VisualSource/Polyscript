#pragma once
#include "../Object.hpp"
namespace jit
{
    class String : public Object
    {
    private:
        std::string value;

    public:
        String(std::string value) : Object(3), value(value) {}
        String() : Object(3), value("") {}
        inline std::string getValue() const { return value; }
        void print(std::ostream &where) const override;
        friend String operator+(String &lhs, const String &rhs);
        friend bool operator==(const String &lhs, const String &rhs);
        inline friend bool operator!=(const String &lhs, const String &rhs) { return !(lhs == rhs); }
    };
} // namespace jit
