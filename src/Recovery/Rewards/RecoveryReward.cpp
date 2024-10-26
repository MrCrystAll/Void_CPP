#include <Recovery/RecoveryUtils.h>
#include <Recovery/Rewards/RecoveryReward.h>
#include <Replays/RCF/RCFUtils.h>

USE_RECOVERY_REWARDS_NS;

void RecoveryReward::PreStep(const GameState& state)
{
	LoggableReward::PreStep(state);

	for (const PlayerData& p : state.players) 
	{

		if (not this->lastHasFlipped.contains(p.carId)) {
			this->lastHasFlipped[p.carId] = p.carState.hasFlipped;
			this->lastIsOnGround[p.carId] = p.carState.isOnGround;
			this->gotFlipFromDash[p.carId] = false;
			this->dashStreaks[p.carId] = 0;
		}
		else {
			if (not p.carState.isOnGround and not p.carState.hasJumped and this->lastHasFlipped[p.carId] and this->lastIsOnGround[p.carId]) {
				this->dashStreaks[p.carId]++;
				this->gotFlipFromDash[p.carId] = true;
			}
			if (not p.carState.hasFlipped) {
				this->gotFlipFromDash[p.carId] = false;
			}
		}

		if (p.carState.hasJumped) {
			this->dashStreaks[p.carId] = 0;
		}

		if (this->delaySinceLastHasFlipped.contains(p.carId)) {
			this->delaySinceLastHasFlipped[p.carId] = std::min(300, this->delaySinceLastHasFlipped[p.carId] + 1);
		}
		else {
			this->delaySinceLastHasFlipped[p.carId] = 1;
		}

	}

}

void RecoveryReward::AddDoubleJumpPunishment(const PlayerData& player) {
	this->reward -= {player.carState.hasDoubleJumped * config.doubleJumpWeight, "Double jump punishment"};
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

Vec GetAbsoluteUp(const PlayerData& player) {
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

	return up;
}

void RecoveryReward::UpFacingReward(const PlayerData& player) {
	Vec up = GetAbsoluteUp(player);
	

	//If the up direction is at least perpendicular to the absolute up, it's good
	this->reward += { up.Dot(player.carState.rotMat.up) > 0.6f and not player.carState.isOnGround ? 0 : -config.facingUpWeight, "Up is facing the absolute upwards direction" };

}

void RecoveryReward::FlipDelayReward(const PlayerData& player) {
	if (player.carState.isFlipping) {
		this->reward += {std::sqrtf(this->delaySinceLastHasFlipped[player.carId] / 10.0f), "Delay between flips"};
	}

	if (player.carState.hasFlipped and not player.carState.isFlipping) this->delaySinceLastHasFlipped[player.carId] = 0;
}

void RecoveryReward::FlipTimeReward(const PlayerData& player)
{
	if (player.carState.flipTime > 0 and player.carState.flipTime < 1) this->reward += { (std::log(-player.carState.flipTime + 1) + 1) * config.flipTimeWeight, "Flip time" };
}

void RecoveryReward::OnlyJumpHeldTooLongPunishment(const PlayerData& player)
{
	if (player.carState.hasJumped and not (player.carState.hasFlipped or player.carState.hasDoubleJumped)) {
		this->delaySinceOnlyJump[player.carId] = std::min(this->delaySinceOnlyJump[player.carId] + 1, 200);
		if (this->delaySinceOnlyJump[player.carId] > 100) {
			this->reward -= {this->delaySinceOnlyJump[player.carId] / 100, "Only jump too long"};
		}
	}
	else {
		this->delaySinceOnlyJump[player.carId] = 0;
	}
}

void RecoveryReward::HeightLimitPunishment(const PlayerData& player)
{
	const int tolerance = 200;
	if (Void::Replays::RCF::isOnCorner(player, tolerance) or Void::Replays::RCF::IsOnWall(player, tolerance, tolerance) or Void::Replays::RCF::IsOnCeiling(player, tolerance, tolerance)) {
		this->reward += {1, "Within height range"};
	}
	else if(player.carState.pos.z < tolerance) {
		this->reward += {1, "Within height range"};
	}
	else {
		this->reward -= {1, "Within height range"};
	}
}

void RecoveryReward::DashStreakReward(const PlayerData& player)
{
	this->reward += { this->dashStreaks.contains(player.carId) and this->dashStreaks[player.carId] > 0 ? std::exp(this->dashStreaks[player.carId]) * 10 : 0, "Dash streak"};
}

void RecoveryReward::DashReward(const PlayerData& player)
{
	this->reward += { this->dashStreaks.contains(player.carId) and this->dashStreaks[player.carId] > 0 ? 100 : 0, "Dash"};
}

void RecoveryReward::DashResultReward(const PlayerData& player)
{
	this->reward += {this->gotFlipFromDash.contains(player.carId) and this->gotFlipFromDash[player.carId] ? (this->lastLinVel[player.carId] - player.phys.vel.Length()) / 200 : 0, "Dash acceleration"};
	this->reward += {this->gotFlipFromDash.contains(player.carId) ? this->gotFlipFromDash[player.carId] * config.dashResultWeight : 0, "Flip from dash"};
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
	this->DashStreakReward(player);

	this->DashReward(player);

	this->DashResultReward(player);

	this->HeightLimitPunishment(player);

	//Facing the ball
	this->reward += {this->faceball->GetReward(player, state, prevAction) * 0.1f, "Facing ball"};

	this->lastIsOnGround[player.carId] = player.carState.isOnGround;
	this->lastHasFlipped[player.carId] = player.carState.hasFlipped;
	this->lastLinVel[player.carId] = player.carState.vel.Length();
	

	return this->ComputeReward();
}

void RecoveryReward::Reset(const GameState& initialState)
{
	LoggableReward::Reset(initialState);
	this->delaySinceLastHasFlipped.clear();
	this->delaySinceOnlyJump.clear();
	this->dashStreaks.clear();
	this->lastHasFlipped.clear();
	this->lastIsOnGround.clear();
	this->gotFlipFromDash.clear();
	this->lastLinVel.clear();
}
