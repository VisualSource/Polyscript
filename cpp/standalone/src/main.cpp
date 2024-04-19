#include <vip/version.h>
#include <iostream>
#include <string.h>
#include "./commands/interpreter.hpp"
#include "./commands/compiler.hpp"

void display_help()
{
    std::cout << "Unknown Command" << std::endl;
    std::cout << "Valid Commands: " << std::endl;
    std::cout << "  run or run SCRIPT" << std::endl;
    std::cout << "  compile SOURCE TARGET" << std::endl;
    std::cout << "  version" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argv[1] == nullptr)
    {
        display_help();
        return 1;
    }

    if (strcmp(argv[1], "run") == 0)
    {
        return command::run_interpreter(argc, argv);
    }
    else if (strcmp(argv[1], "compile") == 0)
    {
        return command::run_compiler(argc, argv);
    }
    else if (strcmp(argv[1], "version") == 0)
    {
        std::cout << "VIP: " << VIP_VERSION << std::endl;
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        display_help();
    }
    else
    {
        display_help();
    }

    return 0;
    /*if (strcmp(argv[1], "run"))
    {
        // return command::run_interpreter(argc, argv);
        return 1;
    }
    else if (strcmp(argv[1], "compile"))
    {
        // return command::run_compiler(argc, argv);
        return 1;
    }*/
}