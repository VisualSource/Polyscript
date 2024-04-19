#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <vip/jit/Object.hpp>

std::shared_ptr<jit::Object> printLn(std::vector<std::shared_ptr<jit::Object>> args)
{
    for (auto &&i : args)
    {
        std::cout << *i;
    }

    std::cout << std::endl;

    return nullptr;
}