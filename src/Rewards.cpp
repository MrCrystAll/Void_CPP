#include "Rewards.h"

PinchReward::PinchReward(float similarityBallAgentThresh, float similarityBallWallThresh, float creepingDistance, float similarityBallAgentReward, float wallMinHeightToPinch, float ballVelW, float speedMatchW, float rewardDecay) :
    similarityBallAgentReward(similarityBallAgentReward),
    similarityBallAgentThresh(similarityBallAgentThresh),
    similarityBallWallThresh(similarityBallWallThresh),
    creepingDistance(creepingDistance),
    wallMinHeightToPinch(wallMinHeightToPinch),
    ballVelW(ballVelW),
    speedMatchW(speedMatchW),
	rewardDecay(rewardDecay)
{
	this->closeToWall = false;
	this->enterCloseWallVel = 0;
	this->lastBallAccel = 0;
	this->lastBallSpeed = 0;
	this->nbStepsSinceLastHit = 0;
}

void PinchReward::Reset(const RLGSC::GameState& initialState)
{
	this->closeToWall = false;
	this->enterCloseWallVel = 0;
	this->lastBallAccel = 0;
	this->lastBallSpeed = 0;
	this->nbStepsSinceLastHit = 0;
}

float PinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	//REWARD
	float reward = 0.f;

	bool lastCloseToWall = closeToWall;
	closeToWall = state.ball.pos.x < -RLGSC::CommonValues::SIDE_WALL_X + 100 + RLGSC::CommonValues::BALL_RADIUS || state.ball.pos.x > RLGSC::CommonValues::SIDE_WALL_X - 100 - RLGSC::CommonValues::BALL_RADIUS;
	
	if (closeToWall) {
		if (enterCloseWallVel == 0) {
			enterCloseWallVel = state.ball.vel.Length2D();
		}
	}
	if (lastCloseToWall and not closeToWall and not player.carState.isOnGround) {

		lastBallAccel = (state.ball.vel.Length2D() - enterCloseWallVel) / RLConst::BALL_MAX_SPEED;
		nbStepsSinceLastHit = 0;
		enterCloseWallVel = 0;
	}
	

	if (lastBallAccel > 0) {
		float ballVelMag2d = state.ball.vel.Length2D();
		float rewardAddition = lastBallAccel * ballVelW - nbStepsSinceLastHit * rewardDecay;
		if (rewardAddition > 0)
		{
			reward += rewardAddition;
			nbStepsSinceLastHit++;
		}
		else {
			lastBallAccel = 0;
			nbStepsSinceLastHit = 0;
		}
	}

	lastBallSpeed = state.ball.vel.Length2D();


    return reward;
}
