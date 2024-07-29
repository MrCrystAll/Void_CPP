#include <TeamPinch/TeamPinchUtils.h>
#include <TeamPinch/Rewards/TeamPinchReward.h>

USE_TEAMPINCH_REWARDS_NS;

void TeamPinchReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float TeamPinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	return this->ComputeReward();
}

void TeamPinchReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}
