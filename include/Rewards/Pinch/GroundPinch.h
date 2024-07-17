/*****************************************************************//**
 * \file   GroundPinch.h
 * \brief  A ground pinch reward
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once

#include <Logging/LoggableReward.h>
#include "Pinch.h"

START_REWARDS_PINCH_NS

/**
 * @brief A reward for ground pinches
 */
class PinchGroundSetupReward : public LoggableReward {
public:
	struct PinchGroundSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchGroundSetupReward(PinchGroundSetupArgs args, std::string name = "Ground pinch reward") : config(args), LoggableReward(name), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchGroundSetupArgs config;
	PinchReward pinchReward;
};
END_PINCH_NS