#pragma once
#include <Recovery/RecoveryUtils.h>

START_RECOVERY_REWARDS_NS

class TimeBetweenFlipsPunishment : public LoggableReward{
public:

	TimeBetweenFlipsPunishment(std::string name = "Time between flips punishment") : LoggableReward(name){};

	// Inherited via LoggableReward
	float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
};

END_RECOVERY_NS