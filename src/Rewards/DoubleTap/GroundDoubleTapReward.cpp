#include "Rewards/DoubleTap/GroundDoubleTapReward.h"

USE_DT_NS

float GroundDoubleTapReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float reward = 0.0f;
	float yDir = player.team == Team::BLUE ? 1 : -1;

	//I think i can do something like "outside of the bb zone, reward for hitting the ball towards the zone"
	if (IsOutsideBackboardZone(state.ball, config.ballZoning.distFromBackboard, config.ballZoning.minHeight)) {

		Vec ballZoneDiff = Vec(state.ball.pos.x, (RLGSC::CommonValues::BACK_WALL_Y - (config.ballZoning.distFromBackboard / 2)) * yDir, (config.ballZoning.minHeight + RLGSC::CommonValues::CEILING_Z) / 2) - state.ball.pos;

		AddLog(reward, "Ball towards zone", state.ball.vel.Normalized().Dot(ballZoneDiff.Normalized()) * config.ballHandling.ballTowardsZoneW);
		AddLog(reward, "Ball dist to zone", -ballZoneDiff.Length() / config.ballHandling.distToZoneReduction);
		AddLog(reward, "Ball touch (outside of bb zone)", player.ballTouchedStep * config.ballHandling.touchW);
		AddLog(reward, "Ball height", state.ball.pos.z / RLGSC::CommonValues::CEILING_Z * config.ballHandling.ballHeightW);
	}

	AddLog(reward, "Double tap total", this->dtReward.GetReward(player, state, prevAction));

	return reward;
}

void GroundDoubleTapReward::ClearChanges()
{
	float temp;
	AddLog(temp, "Ball towards zone", 0, true);
	AddLog(temp, "Ball dist to zone", 0, true);
	AddLog(temp, "Ball touch (outside of bb zone)", 0, true);
	AddLog(temp, "Ball height", 0, true);
	AddLog(temp, "Double tap total", 0, true);
	UseDTReward::ClearChanges();
}
