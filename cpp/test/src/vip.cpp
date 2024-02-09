#include <doctest/doctest.h>
#include <vip/vip.hpp>
#include <vip/jit/Consts.hpp>
#include <vip/jit/components/Number.hpp>

#include <string>

TEST_CASE("Binary Operations")
{
    auto runtime = vip::JustInTime(true);

    SUBCASE("ADDING works as expected")
    {
        auto result = runtime.execute("1 + 1;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 2);
    }

    SUBCASE("Subtraction works as expected")
    {
        auto result = runtime.execute("1 - 1;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 0);
    }

    SUBCASE("mutlply works as expected")
    {
        auto result = runtime.execute("2 * 2;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 4);
    }

    SUBCASE(" works as expected")
    {
        auto result = runtime.execute("2 / 2;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 0);
    }
}
