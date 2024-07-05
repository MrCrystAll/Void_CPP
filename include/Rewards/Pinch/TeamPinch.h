#pragma once
#include "Pinch.h"

START_PINCH_NS

class PinchTeamSetupReward : public LoggableReward {
public:
	struct PinchTeamSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchTeamSetupReward(std::string name, PinchTeamSetupArgs args) : config(args), LoggableReward(name), pinchReward(PinchReward("Pinch", args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchTeamSetupArgs config;
	PinchReward pinchReward;
};

END_PINCH_NS