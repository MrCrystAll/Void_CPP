#pragma once
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>

class PinchReward : public RLGSC::RewardFunction {
public:
	PinchReward(
		float similarityBallAgentThresh = 0.8f,
		float similarityBallWallThresh = 0.8f,
		float creepingDistance = 2000.0f,
		float similarityBallAgentReward = 0.2f,
		float wallMinHeightToPinch = 150,
		float ballVelW = 1.0f,
		float speedMatchW = 1.0f,
		float groundBanDistance = 1000.0f,
		float groundBanPunishment = 0.1f,
		float groudBanReward = 0.1f,
		float maxDistToTrigger = 4000.0f
	);
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
private:
	float lastBallSpeed;
	float similarityBallAgentThresh;
	float similarityBallWallThresh;
	float creepingDistance;
	float similarityBallAgentReward;
	float wallMinHeightToPinch;
	float ballVelW;
	float speedMatchW;
	float groundBanDistance;
	float groundBanPunishment;
	float maxDistToTrigger;
	float groudBanReward;
};