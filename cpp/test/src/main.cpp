#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <vip/vip.hpp>
#include <vip/jit/Consts.hpp>
#include <vip/jit/components/Number.hpp>

#include <string>

TEST_SUITE("Operations")
{
    TEST_CASE("Binary Operations (ADD)")
    {
        auto runtime = vip::JustInTime(true);

        auto result = runtime.execute("1 + 1;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 2);
    }
    TEST_CASE("Binary Operations (MINUS)")
    {
        auto runtime = vip::JustInTime(true);

        auto result = runtime.execute("1 - 1;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 0);
    }
    TEST_CASE("Binary Operations (DIVIDE)")
    {
        auto runtime = vip::JustInTime(true);

        auto result = runtime.execute("1 / 1;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 1);
    }
    TEST_CASE("Binary Operations (MULT)")
    {
        auto runtime = vip::JustInTime(true);

        auto result = runtime.execute("2 * 2;");

        REQUIRE(result->getKind() == jit::consts::ID_NUMBER);

        auto item = std::dynamic_pointer_cast<jit::Number>(result);

        REQUIRE(item != nullptr);

        REQUIRE(item->getValue() == 4);
    }
    TEST_CASE("Binary Operations (DIV BY ZERO)")
    {
        auto runtime = vip::JustInTime(true);
        CHECK_THROWS_AS(runtime.execute("2 / 0;"), const std::overflow_error &);
    }
}
