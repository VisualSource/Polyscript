#include <vip/jit/components/String.hpp>

namespace jit
{
    void String::print(std::ostream &where) const
    {
        where << value;
    }

    String operator+(String &lhs, const String &rhs)
    {
        return String(lhs.getValue() + rhs.getValue());
    }

    bool operator==(const String &lhs, const String &rhs)
    {
        return lhs.getValue() == rhs.getValue();
    }
} // namespace jit
