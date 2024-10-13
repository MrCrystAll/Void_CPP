#include <assert.h>
#include <iostream>
#include <catch2/catch_all.hpp>
#include <TestsUtils.h>
#include <Rewards/RewardsUtils.h>
#include <Rewards/ZeroSumTests.h>

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    // Tests have to meet some requirements to be considered valid
    REQUIRE(true);
}

TEST_CASE("Rewards", VOID_REWARDS_TAG){
}
