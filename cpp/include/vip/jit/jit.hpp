#pragma once
#include "./runtime.hpp"
#include "./components/InternalFunction.hpp"
#include "./Object.hpp"
#include <string>
#include <memory>

namespace jit
{
    class JustInTime
    {
    private:
        Runtime rt;
        bool cliMode;

    public:
        /// @brief Create a runtime wrapper for just in time
        /// @param cliMode should the last statement be printed to std out.
        JustInTime(bool cliMode) : rt(Runtime()), cliMode(cliMode) {}
        JustInTime() : rt(Runtime()), cliMode(false) {}
        /// @brief Register an system level function
        /// @param name name of function
        /// @param callback function to call
        void registerFn(std::string name, CallbackFunction callback);
        /// @brief execute code
        /// @param input the content to execute.
        std::shared_ptr<Object> execute(std::string input);
    };

}