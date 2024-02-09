#pragma once
#include <memory>
#include <string>
#include <map>
#include "./Object.hpp"

namespace jit
{
    class Context
    {
    private:
        std::string name;
        Context *parent;
        std::map<std::string, std::shared_ptr<Object>> variables;
        bool returnable;

    public:
        Context(std::string name, Context *ctx, bool returnable = false) : name(name), parent(ctx), returnable(returnable) {}
        Context(const Context &) = delete;
        ~Context();
        Context &operator=(const Context &) = delete;
        bool canReturn() { return returnable; }
        Context *getParentContext();
        bool remove(std::string key);
        bool has(std::string key);
        std::shared_ptr<Object> get(std::string key);
        bool set(std::string key, std::shared_ptr<Object> value);
    };
} // namespace jit
