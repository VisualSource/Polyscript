#include <vip/vip.hpp>
#include <string>

int main()
{
    // fn test(val: number){ let temp: number = 1; return val + temp; }
    std::string input = "if(true){}else if(true){} else {}";

    vip::vipJit(input);

    return 0;
}