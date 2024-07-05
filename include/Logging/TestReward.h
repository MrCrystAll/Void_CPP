#pragma once
#include "Logging/LoggableReward.h"
#include <RLGymSim_CPP/Utils/CommonValues.h>

using namespace RLGSC;

class DummyReward : public LoggableReward {
public:
	DummyReward(std::string name = "Dummy") : LoggableReward(name) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) {

		/*if (state.ball.pos.z < 100) {
			reward += {1, "Ball height"};
		}*/
		reward += {player.carState.vel.Length() / RLGSC::CommonValues::CAR_MAX_SPEED, "Ball speed"};

		return reward.value;
	}
};