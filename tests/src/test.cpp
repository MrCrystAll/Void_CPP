#include <assert.h>
#include <iostream>
#include <catch2/catch_all.hpp>
#include <TestsUtils.h>
#include <Rewards/RewardsUtils.h>
#include <Replays/ReplaysUtils.h>
#include <Rewards/ZeroSumTests.h>

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    // Tests have to meet some requirements to be considered valid
    REQUIRE(true);
}

TEST_CASE("Utils", VOID_TESTS_TAG) {

}

TEST_CASE("Rewards", VOID_REWARDS_TAG){
}

TEST_CASE("Replay loader", VOID_REPLAYS_TAG) {

}
