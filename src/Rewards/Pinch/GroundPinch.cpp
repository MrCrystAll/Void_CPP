#include "Rewards/Pinch/GroundPinch.h"

USE_PINCH_NS

void PinchGroundSetupReward::Reset(const RLGSC::GameState& initialState)
{
}

float PinchGroundSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float reward = 0.0f;


	/* Pseudo algorithm idea */

	return reward;
}

void PinchGroundSetupReward::Log(RLGPC::Report& report, std::string name, float weight)
{
	LoggableReward::Log(report, name, weight);
	this->pinchReward.Log(report, name + "/Pinch", weight);
}

void PinchGroundSetupReward::ClearChanges()
{
	this->pinchReward.ClearChanges();
}