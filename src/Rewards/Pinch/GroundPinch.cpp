#include "Rewards/Pinch/GroundPinch.h"

USE_PINCH_NS

void PinchGroundSetupReward::Reset(const RLGSC::GameState& initialState)
{
}

float PinchGroundSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	/* Pseudo algorithm idea */

	return this->ComputeReward();
}

void PinchGroundSetupReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}