#pragma once
#include <memory>
#include <string>
#include <fstream>
#include "./targets/target.hpp"
#include "./Object.hpp"

namespace compiler
{
    class Compiler
    {
    private:
        Target *target;

        std::string entrypoint;
        std::string output;

        std::ofstream file;

    public:
        Compiler(Target *target, std::string entrypoint, std::string output) : target(target),
                                                                               entrypoint(entrypoint),
                                                                               output(output),
                                                                               file(std::ofstream(this->output)) {}
        ~Compiler();

        void compile();
    };
}