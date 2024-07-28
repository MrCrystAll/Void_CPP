#pragma once
#include <Recovery/RecoveryUtils.h>

START_RECOVERY_REWARDS_NS

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

END_RECOVERY_NS