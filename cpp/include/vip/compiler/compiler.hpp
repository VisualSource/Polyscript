#pragma once
#include <memory>
#include <string>
#include "./Object.hpp"

namespace compiler
{
    class Compiler
    {
    private:
    public:
        Compiler();
        ~Compiler();
        void declare(std::string key, std::shared_ptr<Object> value);
        void compile(std::string output, std::string target);
    };
}