#pragma once
#include <Logging/LoggableReward.h>
#include <Utils/DashesUtils.h>

START_REWARDS_DASHES_NS

class TimeBetweenFlipsPunishment : public LoggableReward{
public:

	struct TBFPArgs {
		//Weight of the flip timer
		float flipTimerW = 100.0f;
	};

	TimeBetweenFlipsPunishment(TBFPArgs config, std::string name = "Time between flips punishment") : LoggableReward(name), config(config) {};

	// Inherited via LoggableReward
	float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
private:
	TBFPArgs config;
};

END_REWARDS_NS