/*****************************************************************//**
 * \file   TeamPinch.h
 * \brief  A team pinch reward
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include "Pinch.h"

START_REWARDS_PINCH_NS

/**
 * @brief A reward for team pinches
 */
class PinchTeamSetupReward : public LoggableReward {
public:
	struct PinchTeamSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchTeamSetupReward(PinchTeamSetupArgs args, std::string name = "Team pinch reward") : config(args), LoggableReward(name), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchTeamSetupArgs config;
	PinchReward pinchReward;
};

END_PINCH_NS