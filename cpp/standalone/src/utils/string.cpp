#include "./string.hpp"
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace utils
{
    void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                        std::not1(std::ptr_fun<int, int>(std::isspace))));
    }
    void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             std::not1(std::ptr_fun<int, int>(std::isspace)))
                    .base(),
                s.end());
    }
    void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }
    bool ends_with(std::string_view str, std::string_view suffix)
    {
        return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    bool starts_with(std::string_view str, std::string_view prefix)
    {
        return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    }
} // namespace utils