#include "Rewards/Dashes/TimeBetweenFlipsPunishment.h"

float TimeBetweenFlipsPunishment::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
    float flipTime = player.carState.flipTime;
    if (flipTime >= 0.05) this->reward += {-flipTime, "Flip time reward"};
    else this->reward += {-1, "Flip time reward"};

    return this->ComputeReward(); 
}
