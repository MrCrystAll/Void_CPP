#pragma once
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>

class PinchReward : public RLGSC::RewardFunction {
public:
	PinchReward(
		float similarityBallAgentThresh = 0.8f,
		float similarityBallWallThresh = 0.8f,
		float creepingDistance = 800.f,
		float similarityBallAgentReward = 0.2f,
		float wallMinHeightToPinch = 150,
		float ballVelW = 1.0f,
		float speedMatchW = 1.0f,
		float rewardDecay = 0.1f
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

	bool closeToWall;
	float lastBallAccel;
	float rewardDecay;
	float enterCloseWallVel;
	int nbStepsSinceLastHit;
};