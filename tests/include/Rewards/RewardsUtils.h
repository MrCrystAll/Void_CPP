#pragma once

#define VOID_REWARDS_TAG "[Void_Rewards]"

#include <Logging/LoggableReward.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

USE_LOGGING_NS;

class TestReward : public LoggableReward {
public:
	TestReward(float blueReward, float orangeReward, std::string name = "Test reward") : LoggableReward(name), blueReward(blueReward), orangeReward(orangeReward) {};
	// Inherited via LoggableReward
	float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
private:
	float blueReward, orangeReward;
};
