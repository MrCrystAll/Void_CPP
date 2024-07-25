#include "Rewards/Pinch/TeamPinch.h"

USE_REWARDS_PINCH_NS;

void PinchTeamSetupReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float PinchTeamSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	return this->ComputeReward();
}

void PinchTeamSetupReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}