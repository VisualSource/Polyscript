#pragma once
#include <memory>
#include <vector>
#include "../Object.hpp"
namespace jit
{
    typedef std::shared_ptr<Object> (*CallbackFunction)(std::vector<std::shared_ptr<Object>>);
    class InternalFunction : public Object
    {
    private:
        std::string name;
        CallbackFunction func;

    public:
        InternalFunction(std::string name, CallbackFunction callback) : Object(4), name(name), func(callback) {}
        std::shared_ptr<Object> execute(std::vector<std::shared_ptr<Object>> args);
        void print(std::ostream &where) const override;
    };
} // namespace jit
