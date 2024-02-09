#include <vip/vip.hpp>
#include <vip/jit/components/InternalFunction.hpp>
#include <string>
#include <memory>
#include <iostream>
#include "./utils.hpp"

std::shared_ptr<jit::Object> printLn(std::vector<std::shared_ptr<jit::Object>> args)
{
    for (auto &&i : args)
    {
        std::cout << *i << "\n";
    }
    return nullptr;
}

int main()
{
    auto jit = vip::JustInTime(true);

    jit.registerFn("println", printLn);

    std::string input;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        utils::trim(input);

        if (!utils::ends_with(input, ";"))
        {
            input.push_back(';');
        }

        if (input == "exit();")
        {
            break;
        }

        try
        {
            jit.execute(input);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}