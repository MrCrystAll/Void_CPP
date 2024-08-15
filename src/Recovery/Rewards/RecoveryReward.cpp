#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>

USE_RECOVERY_REWARDS_NS;

float RecoveryReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	float normalizedPlayerSpeed = player.carState.vel.Length() / CommonValues::CAR_MAX_SPEED;
	this->reward -= {player.carState.hasDoubleJumped, "Double jump punishment"};
	this->reward += {player.carState.hasFlipped ? std::exp(-4 * player.carState.flipTime) : 0, "Flip time"};
	this->reward -= {player.carState.pos.Dist(state.ball.pos) / 2500, "Distance to ball"};
	this->reward += {std::exp(3 * (normalizedPlayerSpeed - 1)), "Player velocity"};

	return this->ComputeReward();
}
