#include "./interpreter.hpp"
#include <vip/version.h>
#include <vip/jit/jit.hpp>
#include <vip/utils/load_file.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "../utils/string.hpp"
#include "../extentions/jit/println.hpp"

namespace command
{
    int run_interpreter([[maybe_unused]] int argc, char *argv[])
    {
        auto jit = jit::JustInTime(argc == 2);

        jit.registerFn("println", printLn);

        if (argc == 2)
        {
            std::string input;
            std::cout << "Vip " << VIP_VERSION << " | Use exit() to exit process." << std::endl;
            while (true)
            {
                std::cout << "> ";
                std::getline(std::cin, input);

                utils::trim(input);

                if (!utils::ends_with(input, ";"))
                    input.push_back(';');

                if (input == "exit();")
                    break;

                try
                {
                    auto result = jit.execute(input);
                    if (result != nullptr)
                    {
                        std::cout << *result << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }

            return 0;
        }

        std::string content;
        if (!vip::utils::load_file(argv[3], content))
            return 1;

        try
        {
            jit.execute(content);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return 1;
    }
};
