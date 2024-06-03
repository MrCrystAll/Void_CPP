#pragma once
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include "LoggedCombinedReward.h"


class PinchReward : public LoggableReward {
public:

	//All the distances
	struct Distances {
		//Distance for pinch setup
		float creepingDistance = 2000.0f;

		//Distance where you should be jumping
		float groundBanDistance = 1000.0f;

		//Distance where the reward will trigger
		float maxDistToTrigger = 4000.0f;
	};

	//All the flip handling
	struct FlipHandling {
		//Reward for keeping the flip in creeping distance
		float hasFlipReward = 1.0f;

		//Reward for flipping on the ball
		float hasFlipRewardWhenBall = 20.0f;

		//Punishment for using the flip in creeping distance
		float hasFlipPunishment = -1.0f;

		//Punishment for keeping the flip on the ball
		float hasFlipPunishmentWhenBall = -20.0f;

		//Distance to ball min to be considered "on the ball" (outer ball)
		float maxDistance = 100;
	};

	//All the similarity stuff
	struct Similarity {
		//Reward for respecting ball agent parallel
		float similarityBallAgentReward = 0.2f;

		//Threshold for ball agent parallel
		float similarityBallAgentThresh = 0.8f;

		//Threshold for ball wall parallel
		float similarityBallWallThresh = 0.8f;
	};

	//All the ground handling stuff
	struct GroundHandling {
		//Punishment for being on ground in ground ban zone
		float groundBanPunishment = 0.1f;

		//Reward for not being on ground in ground ban zone
		float groundBanReward = -0.1f;

		//Reward for being in creeping distance
		float creepingDistanceReward = 0.01f;
	};

	//All the ball handling stuff
	struct BallHandling {
		//Distance to ball reduction, the lower it is, the more distance to ball is punished
		float ballDistReduction = 1000.0f;

		//Acceleration weight
		float ballVelW = 1.0f;

		//Ball agent speed match
		float speedMatchW = 1.0f;

		//Touch weight
		float touchW = 1.0f;

		//Distance between agent and ball where the agent receives a reward
		float agentDistToBallThresh = 550.0f;

		//Allowed offset off the ball on the x axis
		float ballOffsetX = 150.0f;

		//Allowed offset off the ball on the y axis
		float ballOffsetY = 150.0f;

		float behindTheBallReward = 0.01f;
	};

	//All the wall handling
	struct WallHandling {
		//Minimum height where hit is considered as pinch
		float wallMinHeightToPinch = 150;
	};

	struct PinchArgs {
		Distances distances = {};
		FlipHandling flipHandling = {};
		Similarity similarity = {};
		GroundHandling groundHandling = {};
		BallHandling ballHandling = {};
		WallHandling wallHandling = {};
	};

	PinchReward(
		PinchArgs args
	);

	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void ClearChanges() override;
private:
	Distances distances;
	FlipHandling flipHandling;
	Similarity similarity;
	GroundHandling groundHandling;
	BallHandling ballHandling;
	WallHandling wallHandling;

	float lastBallSpeed = 0;
};