#include <vip/jit/Context.hpp>
#include <iterator>

namespace jit
{
    Context::~Context()
    {
        variables.clear();
    }
    Context *Context::getParentContext()
    {
        return parent;
    }

    bool Context::remove(std::string key)
    {
        return variables.erase(key) == 1;
    }
    bool Context::has(std::string key)
    {
        return variables.find(key) != variables.end();
    }

    bool Context::set(std::string key, std::shared_ptr<Object> value)
    {
        variables.insert({key, std::move(value)});
        return true;
    }

    std::shared_ptr<Object> Context::get(std::string key)
    {
        if (has(key))
        {
            return variables.at(key);
        }

        if (parent != nullptr)
        {
            return parent->get(key);
        }

        return nullptr;
    }
} // namespace jit
