#include "Rewards/DoubleTap/GroundDoubleTapReward.h"

USE_DT_NS

float GroundDoubleTapReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float yDir = player.team == Team::BLUE ? 1 : -1;

	//I think i can do something like "outside of the bb zone, reward for hitting the ball towards the zone"
	if (IsOutsideBackboardZone(state.ball, config.ballZoning.distFromBackboard, config.ballZoning.minHeight)) {

		Vec ballZoneDiff = Vec(state.ball.pos.x, (RLGSC::CommonValues::BACK_WALL_Y - (config.ballZoning.distFromBackboard / 2)) * yDir, (config.ballZoning.minHeight + RLGSC::CommonValues::CEILING_Z) / 2) - state.ball.pos;

		reward += {state.ball.vel.Normalized().Dot(ballZoneDiff.Normalized())* config.ballHandling.ballTowardsZoneW, "Ball towards zone"};
		reward += { -ballZoneDiff.Length() / config.ballHandling.distToZoneReduction, "Ball dist to zone"};
		reward += {player.ballTouchedStep* config.ballHandling.touchW, "Ball touch (outside of bb zone)"};
		reward += {state.ball.pos.z / RLGSC::CommonValues::CEILING_Z * config.ballHandling.ballHeightW, "Ball height"};
	}

	reward += {this->dtReward.GetReward(player, state, prevAction), "Double tap total"};


	return this->ComputeReward();
}