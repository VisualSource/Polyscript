#include <vip/jit/components/Function.hpp>

namespace jit
{
    Function::~Function()
    {
        if (body != nullptr)
            delete body;

        for (auto &&i : params)
        {
            if (i != nullptr)
                delete i;
        }
    }
    void Function::print(std::ostream &where) const
    {
        where << "[function " << name << "]";
    }
} // namespace jit
