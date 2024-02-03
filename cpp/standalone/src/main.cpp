#include <vip/vip.hpp>
#include <string>

int main()
{
    // fn test(val: number){ let temp: number = 1; return val + temp; }
    std::string input = "fn main(){ print(\"What a new message.\"); }";
    // vip::vipJit(input);
    vip::vipCompile(input);

    return 0;
}