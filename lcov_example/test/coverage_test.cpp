#include "system_under_test.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "sending_false", "[lcov-example]" ) 
{
    SystemUnderTest sut{ false, 15 };
    sut.DoSomething();

    REQUIRE(sut.GetFlagValue() == "false");
    REQUIRE(sut.GetContainer().at(2) == 44);
}