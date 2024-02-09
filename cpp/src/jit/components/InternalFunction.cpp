#include <vip/jit/components/InternalFunction.hpp>

namespace jit
{
    std::shared_ptr<Object> InternalFunction::execute(std::vector<std::shared_ptr<Object>> args)
    {
        return func(args);
    }
    void InternalFunction::print(std::ostream &out) const
    {
        out << "[function " << name << "]";
    }
} // namespace jit
