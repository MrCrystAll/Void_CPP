#include <assert.h>
#include <iostream>
#include <catch2/catch_all.hpp>
#include <TestsUtils.h>
#include <Rewards/Rewards.h>

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    // Tests have to meet some requirements to be considered valid
    REQUIRE(true);
}

TEST_CASE("Void rewards", VOID_REWARDS_TAG){
}
