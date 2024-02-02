#include <doctest/doctest.h>
#include <vip/vip.hpp>
#include <vip/version.h>

#include <string>

TEST_CASE("Vip version")
{
    static_assert(std::string_view(VIP_VERSION) == std::string_view("1.0"));
    CHECK(std::string(VIP_VERSION) == std::string("1.0"));
}