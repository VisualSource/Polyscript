#pragma once
#include <string>
#include "./jit/runtime.hpp"
#include "./jit/components/InternalFunction.hpp"

namespace vip
{
    class JustInTime
    {
    private:
        jit::Runtime rt;
        bool cliMode;

    public:
        JustInTime(bool cliMode) : rt(jit::Runtime()), cliMode(cliMode) {}
        JustInTime() : rt(jit::Runtime()), cliMode(false) {}
        void registerFn(std::string name, jit::CallbackFunction callback);
        void execute(std::string input);
    };
}