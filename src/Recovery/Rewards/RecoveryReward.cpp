#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>

USE_RECOVERY_REWARDS_NS;

float RecoveryReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	//Insert your reward logic here

	return this->ComputeReward();
}
