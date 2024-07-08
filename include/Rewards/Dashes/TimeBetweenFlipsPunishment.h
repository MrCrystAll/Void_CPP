#pragma once
#include <Logging/LoggableReward.h>

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
