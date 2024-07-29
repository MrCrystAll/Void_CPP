#include <GroundPinch/GroundPinchUtils.h>
#include <GroundPinch/Rewards/GroundPinchReward.h>

USE_GROUNDPINCH_REWARDS_NS;

void GroundPinchReward::Reset(const RLGSC::GameState& initialState)
{
}

float GroundPinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	/* Pseudo algorithm idea */

	return this->ComputeReward();
}

void GroundPinchReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}
