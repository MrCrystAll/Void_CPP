#pragma once

#include "../../LoggedCombinedReward.h"
#include "Pinch.h"

START_PINCH_NS
class PinchGroundSetupReward : public LoggableReward {
public:
	struct PinchGroundSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchGroundSetupReward(PinchGroundSetupArgs args) : config(args), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void ClearChanges() override;
	virtual void Log(RLGPC::Report& report, std::string name, float weight = 1.0f) override;

private:
	PinchGroundSetupArgs config;
	PinchReward pinchReward;
};
END_PINCH_NS