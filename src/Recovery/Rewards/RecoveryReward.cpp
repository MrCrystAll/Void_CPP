#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>

USE_RECOVERY_REWARDS_NS;

float RecoveryReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	float normalizedPlayerSpeed = player.carState.vel.Length() / CommonValues::CAR_MAX_SPEED;
	this->reward -= {player.carState.hasDoubleJumped * config.doubleJumpWeight, "Double jump punishment"};
	this->reward += {((player.carState.hasFlipped ? std::exp(-4 * player.carState.flipTime) : 0) * normalizedPlayerSpeed) * config.flipTimeWeight, "Flip time"};
	this->reward -= {(player.carState.pos.Dist(state.ball.pos) / 5000) * config.distanceToBallWeight, "Distance to ball"};
	if (player.carState.flipTime < 0.5 and player.carState.flipTime > 0) {
		this->reward += {std::exp(3 * (normalizedPlayerSpeed - 1)) * config.velocityWeight, "Player velocity"};
	}

	return this->ComputeReward();
}
