#include <Recovery/Rewards/IsFlippingReward.h>

USE_RECOVERY_REWARDS_NS;

float IsFlippingReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	this->reward += {player.carState.isFlipping, "Is flipping"};
	return this->ComputeReward();
}