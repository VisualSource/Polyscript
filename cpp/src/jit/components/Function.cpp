#include <vip/jit/components/Function.hpp>

namespace jit
{
    void Function::print(std::ostream &where) const
    {
        where << "[function " << name << "]";
    }
} // namespace jit
