#pragma once

#include <Rewards/RewardsUtils.h>
#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <Logging/LoggableReward.h>
#include <vector>

USE_LOGGING_NS;

GameState create3v3State() {
	GameState state = {};

	std::vector<PlayerData> pDatas = {};
	for (uint8_t i = 0; i < 3; i++)
	{
		PlayerData pData = {};
		pData.team = Team::BLUE;
		pDatas.push_back(pData);
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		PlayerData pData = {};
		pData.team = Team::ORANGE;
		pDatas.push_back(pData);
	}

	state.players = pDatas;

	return state;
}

TEST_CASE("ZeroSumTests - Team spirit = 0 && Opponent scaling = 0 | Even rewards", VOID_REWARDS_TAG){

	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(3, 3), 0.0, 0.0);
	
	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 3);
	CHECK(rewards[1] == 3);
	CHECK(rewards[2] == 3);
	CHECK(rewards[3] == 3);
	CHECK(rewards[4] == 3);
	CHECK(rewards[5] == 3);

}

TEST_CASE("ZeroSumTests - Team spirit = 0 && Opponent scaling = 1 | Even rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(3, 3), 0.0, 1.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 0);
	CHECK(rewards[1] == 0);
	CHECK(rewards[2] == 0);
	CHECK(rewards[3] == 0);
	CHECK(rewards[4] == 0);
	CHECK(rewards[5] == 0);
}

TEST_CASE("ZeroSumTests - Team spirit = 1 && Opponent scaling = 0 | Even rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(3, 3), 1.0, 0.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 3);
	CHECK(rewards[1] == 3);
	CHECK(rewards[2] == 3);
	CHECK(rewards[3] == 3);
	CHECK(rewards[4] == 3);
	CHECK(rewards[5] == 3);
}

TEST_CASE("ZeroSumTests - Team spirit = 1 && Opponent scaling = 1 | Even rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(3, 3), 1.0, 1.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 0);
	CHECK(rewards[1] == 0);
	CHECK(rewards[2] == 0);
	CHECK(rewards[3] == 0);
	CHECK(rewards[4] == 0);
	CHECK(rewards[5] == 0);
}

TEST_CASE("ZeroSumTests - Team spirit = 0.46 && Opponent scaling = 0.79 | Even rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(3, 3), 0.46, 0.79);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK_THAT(rewards[0], Catch::Matchers::WithinAbs(0.63, 0.0001));
	CHECK_THAT(rewards[1], Catch::Matchers::WithinAbs(0.63, 0.0001));
	CHECK_THAT(rewards[2], Catch::Matchers::WithinAbs(0.63, 0.0001));
	CHECK_THAT(rewards[3], Catch::Matchers::WithinAbs(0.63, 0.0001));
	CHECK_THAT(rewards[4], Catch::Matchers::WithinAbs(0.63, 0.0001));
	CHECK_THAT(rewards[5], Catch::Matchers::WithinAbs(0.63, 0.0001));
}

TEST_CASE("ZeroSumTests - Team spirit = 0 && Opponent scaling = 0 | Uneven rewards", VOID_REWARDS_TAG) {

	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(1, 2), 0.0, 0.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 1);
	CHECK(rewards[1] == 1);
	CHECK(rewards[2] == 1);
	CHECK(rewards[3] == 2);
	CHECK(rewards[4] == 2);
	CHECK(rewards[5] == 2);

}

TEST_CASE("ZeroSumTests - Team spirit = 0 && Opponent scaling = 1 | Uneven rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(1, 2), 0.0, 1.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == -1);
	CHECK(rewards[1] == -1);
	CHECK(rewards[2] == -1);
	CHECK(rewards[3] == 1);
	CHECK(rewards[4] == 1);
	CHECK(rewards[5] == 1);
}

TEST_CASE("ZeroSumTests - Team spirit = 1 && Opponent scaling = 0 | Uneven rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(1, 2), 1.0, 0.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == 1);
	CHECK(rewards[1] == 1);
	CHECK(rewards[2] == 1);
	CHECK(rewards[3] == 2);
	CHECK(rewards[4] == 2);
	CHECK(rewards[5] == 2);
}

TEST_CASE("ZeroSumTests - Team spirit = 1 && Opponent scaling = 1 | Uneven rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(1, 2), 1.0, 1.0);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK(rewards[0] == -1);
	CHECK(rewards[1] == -1);
	CHECK(rewards[2] == -1);
	CHECK(rewards[3] == 1);
	CHECK(rewards[4] == 1);
	CHECK(rewards[5] == 1);
}

TEST_CASE("ZeroSumTests - Team spirit = 0.46 && Opponent scaling = 0.79 | Uneven rewards", VOID_REWARDS_TAG) {
	ZeroSumLoggedWrapper wrapper = ZeroSumLoggedWrapper(new TestReward(1, 2), 0.46, 0.79);

	FList rewards = wrapper.GetAllRewards(create3v3State(), {}, false);

	CHECK(rewards.size() == 6);
	CHECK_THAT(rewards[0], Catch::Matchers::WithinAbs(-0.58, 0.0001));
	CHECK_THAT(rewards[1], Catch::Matchers::WithinAbs(-0.58, 0.0001));
	CHECK_THAT(rewards[2], Catch::Matchers::WithinAbs(-0.58, 0.0001));
	CHECK_THAT(rewards[3], Catch::Matchers::WithinAbs(1.21, 0.0001));
	CHECK_THAT(rewards[4], Catch::Matchers::WithinAbs(1.21, 0.0001));
	CHECK_THAT(rewards[5], Catch::Matchers::WithinAbs(1.21, 0.0001));
}