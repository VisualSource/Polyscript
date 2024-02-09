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

    std::shared_ptr<Object> Context::set(std::string key, std::shared_ptr<Object> value)
    {
        variables.insert({key, std::move(value)});
        return variables.at(key);
    }

    std::shared_ptr<Object> Context::update(std::string key, std::shared_ptr<Object> value)
    {
        if (has(key))
        {
            variables[key] = value;
            return variables.at(key);
        }

        if (parent != nullptr)
        {
            return parent->update(key, value);
        }

        return nullptr;
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
