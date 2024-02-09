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
        /// @brief Create a runtime wrapper for just in time
        /// @param cliMode should the last statement be printed to std out.
        JustInTime(bool cliMode) : rt(jit::Runtime()), cliMode(cliMode) {}
        JustInTime() : rt(jit::Runtime()), cliMode(false) {}
        /// @brief Register an system level function
        /// @param name name of function
        /// @param callback function to call
        void registerFn(std::string name, jit::CallbackFunction callback);
        /// @brief execute code
        /// @param input the content to execute.
        void execute(std::string input);
    };
}