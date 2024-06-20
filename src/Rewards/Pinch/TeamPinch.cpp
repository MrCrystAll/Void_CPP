#include "Rewards/Pinch/TeamPinch.h"

USE_PINCH_NS

void PinchTeamSetupReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float PinchTeamSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	return 0.0f;
}

void PinchTeamSetupReward::ClearChanges()
{
	this->pinchReward.ClearChanges();
}

void PinchTeamSetupReward::Log(RLGPC::Report& report, std::string name, float weight)
{
	LoggableReward::Log(report, name, weight);
	this->pinchReward.Log(report, name, weight);
}