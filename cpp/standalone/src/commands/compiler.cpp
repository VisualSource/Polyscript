#include "./compiler.hpp"
#include <vip/compiler/compiler.hpp>
#include <vip/compiler/targets/target.hpp>
#include <vip/compiler/targets/chip8.hpp>
#include <vip/utils/load_file.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

namespace command
{
    compiler::Target *get_target(std::string target)
    {
        if (target == "chip8-unknown-unknown")
        {
            return (compiler::Target *)(new compiler::X16ChipUnknown());
        }

        throw std::logic_error("Invalid target");
    }

    int run_compiler([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
    {

        if (argc != 5 || argv[2] == nullptr || argv[3] == nullptr || argv[4] == nullptr)
        {
            std::cerr << "Missing argument(s)" << std::endl;
            return 1;
        }

        try
        {
            compiler::Target *target = get_target(argv[4]);

            compiler::Compiler cp(target, argv[2], argv[3]);
            cp.compile();

            return 0;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';

            return 1;
        }
    }
} // namespace command