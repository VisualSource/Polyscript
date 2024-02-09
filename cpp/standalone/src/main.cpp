#include <vip/vip.hpp>
#include <vip/version.h>
#include <vip/jit/components/InternalFunction.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include "./utils.hpp"

std::shared_ptr<jit::Object> printLn(std::vector<std::shared_ptr<jit::Object>> args)
{
    for (auto &&i : args)
    {
        std::cout << *i;
    }

    std::cout << std::endl;

    return nullptr;
}

int main(int argc, char *argv[])
{
    auto jit = vip::JustInTime(argc == 1);

    jit.registerFn("println", printLn);

    if (argc == 1)
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

    std::ifstream file(argv[1]);

    if (!file.is_open())
        return 1;

    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
        content += line;
    }
    file.close();

    try
    {
        jit.execute(content);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}