#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>

USE_RECOVERY_REWARDS_NS;

float RecoveryReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	int numWheels = 0;
	for (bool wheelContact : player.carState.wheelsWithContact) {
		numWheels += wheelContact;
	}

	this->reward += {(numWheels <= 3 and numWheels > 0 and player.carState.isFlipping), "Flipping on less than 4 wheels and on 'ground'"};
	this->reward -= {player.carState.hasFlipped ? player.carState.flipTime + (player.carState.vel.Length() / CommonValues::CAR_MAX_SPEED - 0.5) : 0, "Flip time (weighted with velocity)"};
	this->reward -= {player.carState.pos.Dist(state.ball.pos) / 5000, "Distance to ball"};

	return this->ComputeReward();
}
