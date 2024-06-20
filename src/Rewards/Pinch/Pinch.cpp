#include "Rewards/Pinch/Pinch.h"

USE_PINCH_NS

PinchReward::PinchReward(
	PinchArgs args
) :
	config(args)
{
	this->lastBallSpeed = 0;
}

void PinchReward::Reset(const RLGSC::GameState& initialState)
{
	this->lastBallSpeed = 0;
}

float PinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	//REWARD
	float reward = 0.f;
	float ballAccel = (state.ball.vel.Length2D() - lastBallSpeed) / RLConst::BALL_MAX_SPEED;
	AddLog(reward, "Ball touch", config.ballHandling.touchW);
	AddLog(reward, "Ball accel", ballAccel * config.ballHandling.ballVelW);

	float ballGoalSimilarity = ((player.team == Team::ORANGE ? RLGSC::CommonValues::BLUE_GOAL_CENTER : RLGSC::CommonValues::ORANGE_GOAL_CENTER) - state.ball.pos).Normalized().Dot(state.ball.vel.Normalized());
	if (ballGoalSimilarity < config.ballHandling.goalDirectionSimilarity) {
		AddLog(reward, "Towards net", config.ballHandling.goalDirectionW);
	}

	lastBallSpeed = state.ball.vel.Length2D();
	return reward;
}

void PinchReward::ClearChanges()
{
	float temp = 0;
	LoggableReward::ClearChanges();
	AddLog(temp, "Ball touch", 0, true);
	AddLog(temp, "Ball accel", 0, true);
	AddLog(temp, "Towards net", 0, true);


}