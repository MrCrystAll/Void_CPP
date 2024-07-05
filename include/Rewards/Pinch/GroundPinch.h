#pragma once

#include <Logging/LoggableReward.h>
#include "Pinch.h"

START_PINCH_NS
class PinchGroundSetupReward : public LoggableReward {
public:
	struct PinchGroundSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchGroundSetupReward(std::string name, PinchGroundSetupArgs args) : config(args), LoggableReward(name), pinchReward(PinchReward("Pinch", args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchGroundSetupArgs config;
	PinchReward pinchReward;
};
END_PINCH_NS