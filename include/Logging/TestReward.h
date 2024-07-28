/*****************************************************************//**
 * \file   TestReward.h
 * \brief  A test reward
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include <Logging/LoggableReward.h>
#include <RLGymSim_CPP/Utils/CommonValues.h>

START_LOGGING_NS

using namespace RLGSC;

/**
 * @brief A reward used for testing purposes
 */
class DummyReward : public LoggableReward {
public:
	DummyReward(std::string name = "Dummy") : LoggableReward(name) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) {

		/*if (state.ball.pos.z < 100) {
			reward += {1, "Ball height"};
		}*/
		reward += {player.carState.vel.Length() / RLGSC::CommonValues::CAR_MAX_SPEED, "Ball speed"};

		return this->ComputeReward();
	}
};

END_LOGGING_NS
