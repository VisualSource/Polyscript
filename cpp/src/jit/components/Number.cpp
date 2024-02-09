#include <vip/jit/components/Number.hpp>
#include <stdexcept>

namespace jit
{
    void Number::print(std::ostream &where) const
    {
        if (isBool)
        {
            where << ((value == 1) ? "true" : "false");
        }
        else
        {
            where << value;
        }
    }

    Number operator+(Number &lhs, const Number &rhs)
    {
        return Number(lhs.getValue() + rhs.getValue());
    }

    Number operator-(Number &lhs, const Number &rhs)
    {
        return Number(lhs.getValue() - rhs.getValue());
    }

    Number operator*(Number &lhs, const Number &rhs)
    {
        return Number(lhs.getValue() * rhs.getValue());
    }

    Number operator/(Number &lhs, const Number &rhs)
    {
        if (rhs.getValue() < 0)
            throw std::overflow_error("Divide by zero exception");
        return Number(lhs.getValue() / rhs.getValue());
    }

    bool operator<(const Number &lhs, const Number &rhs)
    {
        return lhs.getValue() < rhs.getValue();
    }

    bool operator==(const Number &lhs, const Number &rhs)
    {
        return lhs.getValue() == rhs.getValue();
    }
}