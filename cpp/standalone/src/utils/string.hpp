#pragma once
#include <string>
#include <string_view>

namespace utils
{
    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
    bool ends_with(std::string_view str, std::string_view suffix);
    bool starts_with(std::string_view str, std::string_view prefix);
} // namespace utils
