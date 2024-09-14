#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>
#include <Replays/RCF/RCFUtils.h>

USE_RECOVERY_REWARDS_NS;

void RecoveryReward::PreStep(const GameState& state)
{
	LoggableReward::PreStep(state);
	this->delaySinceLastHasFlipped = std::min(300, this->delaySinceLastHasFlipped + 1);
}

void RecoveryReward::AddDoubleJumpPunishment(const PlayerData& player) {
	this->reward -= {player.carState.hasDoubleJumped * config.doubleJumpWeight, "Double jump punishment"};
}

void RecoveryReward::FlîpTimeReward(const PlayerData& player) {
	float normalizedPlayerSpeed = player.carState.vel.Length() / CommonValues::CAR_MAX_SPEED;
	this->reward += {((player.carState.hasFlipped ? std::exp(-4 * player.carState.flipTime) : 0) * normalizedPlayerSpeed) * config.flipTimeWeight, "Flip time"};
}

void RecoveryReward::DistToBall(const PlayerData& player, const GameState& state) {
	this->reward -= {(player.carState.pos.Dist(state.ball.pos) / 5000) * config.distanceToBallWeight, "Distance to ball"};
}

void RecoveryReward::VelocityReward(const PlayerData& player) {
	if (player.carState.flipTime < 0.3 and player.carState.flipTime > 0) {
		float normalizedPlayerSpeed = player.carState.vel.Length() / CommonValues::CAR_MAX_SPEED;
		this->reward += {std::exp(3 * (normalizedPlayerSpeed - 1)) * config.velocityWeight, "Player velocity"};
	}
}

void RecoveryReward::UpFacingReward(const PlayerData& player) {
	Vec up = Vec(0, 0, 1);

	bool isOnWall = Void::Replays::RCF::IsOnWall(player);
	bool isOnCeiling = Void::Replays::RCF::IsOnCeiling(player);
	bool isOnCorner = Void::Replays::RCF::isOnCorner(player);

	if (isOnWall) up = player.carState.pos.x < 0 ? Vec(1, 0, 0) : Vec(-1, 0, 0);
	if (isOnCeiling) up = -up;
	if (isOnCorner) {
		float directionFrom45Deg = std::sqrt(2.0f) / 2;
		if (player.carState.pos.x > 0) {
			if (player.carState.pos.y > 0) {
				up = Vec(-directionFrom45Deg, -directionFrom45Deg, 0);
			}
			else {
				up = Vec(directionFrom45Deg, -directionFrom45Deg, 0);
			}
		}
		else {
			if (player.carState.pos.y > 0) {
				up = Vec(-directionFrom45Deg, directionFrom45Deg, 0);
			}
			else {
				up = Vec(directionFrom45Deg, directionFrom45Deg, 0);
			}
		}
	}

	//If the up direction is at least perpendicular to the absolute up, it's good
	this->reward += { up.Dot(player.carState.rotMat.up) > 0.6f and not player.carState.isOnGround ? 0 : -config.facingUpWeight, "Up is facing the absolute upwards direction" };

}

void RecoveryReward::FlipDelayReward(const PlayerData& player) {
	if (player.carState.isFlipping) {
		this->reward += {std::logf(-(this->delaySinceLastHasFlipped / 330) + 1) + 1, "Delay between flips"};
	}

	if (player.carState.hasFlipped and not player.carState.isFlipping) this->delaySinceLastHasFlipped = 0;
}

void RecoveryReward::FlipTimeReward(const PlayerData& player)
{
	if (player.carState.flipTime > 0 and player.carState.flipTime < 1) this->reward += { (std::log(-player.carState.flipTime + 1) + 1) * config.flipTimeWeight, "Flip time" };
}

void RecoveryReward::OnlyJumpHeldTooLongPunishment(const PlayerData& player)
{
	if (player.carState.hasJumped and not (player.carState.hasFlipped or player.carState.hasDoubleJumped)) {
		this->delaySinceOnlyJump = std::min(this->delaySinceOnlyJump + 1, 200);
		if (this->delaySinceOnlyJump > 100) {
			this->reward -= {this->delaySinceOnlyJump / 100, "Only jump too long"};
		}
	}
	else {
		this->delaySinceOnlyJump = 0;
	}
}

float RecoveryReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	//Avoid double jump
	this->AddDoubleJumpPunishment(player);

	//Distance to ball
	//this->DistToBall(player, state);

	//Player velocity if flip time is short
	this->VelocityReward(player);

	//Facing in the wanted direction
	this->UpFacingReward(player);

	//Flip delay reward
	//this->FlipDelayReward(player);
	this->FlipTimeReward(player);

	//Only jump is held too long
	//this->OnlyJumpHeldTooLongPunishment(player);

	//Facing the ball
	this->reward += {this->faceball->GetReward(player, state, prevAction) * 0.1f, "Facing ball"};

	return this->ComputeReward();
}
