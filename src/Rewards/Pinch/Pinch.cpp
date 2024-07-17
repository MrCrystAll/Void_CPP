#include "Rewards/Pinch/Pinch.h"

USE_REWARDS_PINCH_NS;

PinchReward::PinchReward(PinchArgs args, std::string name) :
	config(args),
	LoggableReward(name)
{
	this->lastBallSpeed = 0;
}


void PinchReward::Reset(const RLGSC::GameState& initialState)
{
	this->lastBallSpeed = 0;
}

float PinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float ballAccel = (state.ball.vel.Length2D() - lastBallSpeed) / RLConst::BALL_MAX_SPEED;

	reward += {config.ballHandling.touchW, "Ball touch"};
	reward += {std::exp(ballAccel / config.ballHandling.ballAccelExpReducer)* config.ballHandling.ballVelW, "Ball accel"};
	reward += {player.carState.isFlipping* config.ballHandling.isFlippingW, "Is flipping"};

	float ballGoalSimilarity = ((player.team == Team::ORANGE ? RLGSC::CommonValues::BLUE_GOAL_CENTER : RLGSC::CommonValues::ORANGE_GOAL_CENTER) - state.ball.pos).Normalized().Dot(state.ball.vel.Normalized());
	if (ballGoalSimilarity > config.ballHandling.goalDirectionSimilarity) {
		reward += {std::exp(ballGoalSimilarity * 50 / config.ballHandling.ballDirectionExpReducer)* config.ballHandling.goalDirectionW, "Towards net"};
	}

	lastBallSpeed = state.ball.vel.Length2D();
	return this->ComputeReward();
}