#pragma once
#include <string>
#include "./ast/Parser.hpp"

namespace vip
{
    ast::Program tokenize(std::string input);
}