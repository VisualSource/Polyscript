#include <vip/vip.hpp>
#include <string>

int main()
{
    std::string input = "1 + 1;";

    vip::vipJit(input);

    return 0;
}