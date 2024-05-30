#include "Rewards.h"

PinchReward::PinchReward(float similarityBallAgentThresh, float similarityBallWallThresh, float creepingDistance,
	float similarityBallAgentReward,
	float wallMinHeightToPinch,
	float ballVelW,
	float speedMatchW, float groundBanDistance, float groundBanPunishment, float groudBanReward, float maxDistToTrigger) :
    similarityBallAgentReward(similarityBallAgentReward),
    similarityBallAgentThresh(similarityBallAgentThresh),
    similarityBallWallThresh(similarityBallWallThresh),
    creepingDistance(creepingDistance),
    wallMinHeightToPinch(wallMinHeightToPinch),
    ballVelW(ballVelW),
    speedMatchW(speedMatchW),
	groundBanDistance(groundBanDistance),
	groundBanPunishment(groundBanPunishment),
	groudBanReward(groudBanReward),
	maxDistToTrigger(maxDistToTrigger)
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

	auto ballDir = state.ball.pos.Normalized();
	auto agentDir = player.carState.vel.Normalized();
	short targetDir = ballDir.x < 0 ? -1 : 1;

	float wallInterceptionY = (ballDir.y / ballDir.x) * (RLGSC::CommonValues::SIDE_WALL_X * targetDir - state.ball.pos.x) + state.ball.pos.y;
	Vec interceptPoint = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, wallInterceptionY, 0);
	
	//First thing, detect if we trigger based on ball dist to wall intercept
	if (state.ball.pos.Dist2D(interceptPoint) < maxDistToTrigger) {
		//Testing if before creeping
		if (state.ball.pos.Dist2D(interceptPoint) > creepingDistance) {
			//Match direction and speed
			float speedDiff = std::max(0.01f, std::abs(state.ball.vel.Dist(player.carState.vel)) / RLGSC::CommonValues::SUPERSONIC_THRESHOLD);
			float directionSimilarity = agentDir.Dot(ballDir);

			reward += 1 / pow(speedDiff, 0.1f) * speedMatchW;
			reward += directionSimilarity >= similarityBallAgentThresh ? similarityBallAgentReward : 0;
		}

		//Creeping distance
		else {
			if (interceptPoint.Dist2D(player.carState.pos) <= groundBanDistance) {
				reward -= player.carState.isOnGround ? groundBanPunishment : -groudBanReward;
			}

			//Accel reward on ball hit
			if (player.ballTouchedStep and state.ball.pos.z >= wallMinHeightToPinch) {
				float ballAccel = (state.ball.vel.Length2D() - lastBallSpeed) / RLConst::BALL_MAX_SPEED;
				reward += ballAccel * ballVelW;
				
			}
		}


	}


	lastBallSpeed = state.ball.vel.Length2D();


    return reward;
}
