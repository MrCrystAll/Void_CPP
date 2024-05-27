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

	auto ballDir = state.ball.vel.Normalized();
	short targetDir = ballDir.x < 0 ? -1 : 1;

	float wallInterceptionY = (ballDir.y / ballDir.x) * (RLGSC::CommonValues::SIDE_WALL_X * targetDir - state.ball.pos.x) + state.ball.pos.y;
	Vec interceptPoint = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, wallInterceptionY, 0);

	Vec flatBall = Vec(state.ball.pos.x, state.ball.pos.y, 0);
	bool lastCloseToWall = closeToWall;
	closeToWall = state.ball.pos.x < -RLGSC::CommonValues::SIDE_WALL_X + 100 + RLGSC::CommonValues::BALL_RADIUS || state.ball.pos.x > RLGSC::CommonValues::SIDE_WALL_X - 100 - RLGSC::CommonValues::BALL_RADIUS;

	Vec diffVec = flatBall - interceptPoint;
	float mag2d = sqrtf(powf(diffVec.x, 2) + powf(diffVec.y, 2));

	if (mag2d > creepingDistance and not closeToWall and lastBallAccel <= 0) {
		//Is ball going in the right direction (assuming we want wall pinches)
		if (ballDir.Dot(Vec(targetDir, 0, 0)) > similarityBallWallThresh) {
			Vec agentDir = player.carState.vel.Normalized();

			//Is the player in the same direction as the ball
			if (ballDir.Dot(agentDir) > similarityBallAgentThresh)
			{
				//Dis some good shit
				reward += similarityBallAgentReward;
				reward += 1 / std::max(0.1f, std::abs(state.ball.vel.Length2D() - player.carState.vel.Length2D()) / RLConst::BALL_MAX_SPEED) * speedMatchW;
			}
		}
	}
	else if (closeToWall) {
		if (enterCloseWallVel == 0) {
			enterCloseWallVel = state.ball.vel.Length2D();
		}
	}
	if (lastCloseToWall and not closeToWall) {

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
