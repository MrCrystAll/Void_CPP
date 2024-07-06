/*****************************************************************//**
 * \file   Pinch.h
 * \brief  The base pinch reward
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Logging/LoggableReward.h>
#include "Utils/PinchUtils.h"

START_PINCH_NS

/**
 * @brief Base pinch reward
 */
class PinchReward : public LoggableReward {
public:


	//All the ball handling stuff
	struct BallHandling {
		//Acceleration weight
		float ballVelW = 1.0f;

		//Touch weight
		float touchW = 1.0f;

		//Minimum similarity to match goal direction
		float goalDirectionSimilarity = 0.8f;

		//Reward for going towards the net
		float goalDirectionW = 3.0f;

		//Reward for flipping on the ball
		float isFlippingW = 3.0f;

		//Divide the exponential input to avoid big values
		float ballAccelExpReducer = 2.0f;

		//Divide the exponential input to avoid big values
		float ballDirectionExpReducer = 2.0f;
	};

	struct PinchArgs {
		BallHandling ballHandling = {};
	};

	PinchReward(
		PinchArgs args,
		std::string name = "Pinch reward"
	);

	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
private:
	PinchArgs config;

	float lastBallSpeed = 0;
};

END_PINCH_NS