/*****************************************************************//**
 * \file   GroundDoubleTapReward.h
 * \brief  Contains the ground double tap reward
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once

#include "Rewards/DoubleTap/DoubleTapReward.h"

START_REWARDS_DT_NS

/**
 * @brief A reward for ground double taps
 */
class GroundDoubleTapReward : public UseDTReward {
public:
	struct BallZoning {
		//Distance from back wall where you are consider in the backboard zone
		float distFromBackboard = 500.0f;

		//Minimum height to be considered in the backboard zone
		float minHeight = RLGSC::CommonValues::GOAL_HEIGHT;
	};

	struct BallHandling {
		//Weight for similarity towards zone, ball vel
		float ballTowardsZoneW = 2.0f;

		//Distance ball - zone reduction
		float distToZoneReduction = 100.0f;

		//Touch weight
		float touchW = 30.0;

		//Ball height weight
		float ballHeightW = 3.0;
	};

	struct GroundDTArgs {
		BallZoning ballZoning;
		BallHandling ballHandling;
	};

	GroundDoubleTapReward(GroundDTArgs config, DoubleTapReward::DoubleTapArgs dtConfig, std::string name = "Ground double tap reward") : config(config), UseDTReward(name, dtConfig) {};

	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
private:
	GroundDTArgs config;
};
END_DT_NS