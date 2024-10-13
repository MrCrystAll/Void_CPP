#pragma once

#include <Rewards/RewardsUtils.h>

using namespace RLGSC;

float TestReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
    this->reward += {player.team == Team::BLUE ? this->blueReward : this->orangeReward, "Bonus"};
    return this->ComputeReward();
}
