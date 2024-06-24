#pragma once

#include "../../LoggedCombinedReward.h"
#include "Pinch.h"
#include "PinchUtils.h"

START_PINCH_NS

class PinchWallSetupReward : public LoggableReward {
public:
	//All the wall handling
	struct WallHandling {
		//Minimum height where hit is considered as pinch
		float wallMinHeightToPinch = 150;
	};

	//All the similarity stuff
	struct SimilarityWallSetup {
		//Reward for respecting ball agent parallel
		float similarityBallAgentReward = 0.2f;

		//Threshold for ball agent parallel
		float similarityBallAgentThresh = 0.8f;

		//Threshold for ball wall parallel
		float similarityBallWallThresh = 0.8f;
	};

	//All the ball handling stuff
	struct BallWallSetupHandling {
		//Distance to ball reduction, the lower it is, the more distance to ball is punished
		float ballDistReduction = 1000.0f;

		//Ball agent speed match
		float speedMatchW = 1.0f;

		//Distance between agent and ball where the agent receives a reward
		float agentDistToBallThresh = 550.0f;

		//Allowed offset off the ball on the x axis
		float ballOffsetX = 150.0f;

		//Allowed offset off the ball on the y axis
		float ballOffsetY = 150.0f;

		//Reward for being behind the ball
		float behindTheBallReward = 0.01f;
	};

	//All the ground handling stuff
	struct GroundWallSetupHandling {
		//Punishment for being on ground in ground ban zone
		float groundBanPunishment = 0.1f;

		//Reward for not being on ground in ground ban zone
		float groundBanReward = -0.1f;

		//Reward for being in creeping distance
		float creepingDistanceReward = 0.01f;
	};

	//All the distances
	struct DistancesWallSetup {
		//Distance for pinch setup
		float creepingDistance = 2000.0f;

		//Distance where you should be jumping
		float groundBanDistance = 1000.0f;

		//Distance where the reward will trigger
		float maxDistToTrigger = 4000.0f;

		//Distance where the intercept point becomes unchanged (until ball reaches ground level or player hit)
		float pinchDistance = 400.0f;
	};

	//All the flip handling
	struct FlipHandlingWallSetup {
		//Reward for keeping the flip in creeping distance
		float hasFlipReward = 1.0f;

		//Punishment for using the flip in creeping distance
		float hasFlipPunishment = -1.0f;

		//Distance to ball min to be considered "on the ball" (outer ball)
		float maxDistance = 100;
	};

	struct PinchWallSetupArgs {
		DistancesWallSetup distancesWallSetup = {};
		FlipHandlingWallSetup flipHandlingWallSetup = {};
		SimilarityWallSetup similarityWallSetup = {};
		GroundWallSetupHandling groundWallSetupHandling = {};
		BallWallSetupHandling ballWallHandling = {};
		WallHandling wallHandling = {};

		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	PinchWallSetupReward(
		PinchWallSetupArgs args
	) : config(args), pinchReward(PinchReward(args.pinchRewardConfig)), lastIntercept(Vec()) {};

	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void ClearChanges() override;
	virtual void Log(RLGPC::Report& report, std::string name, float weight = 1.0f) override;

private:
	PinchWallSetupArgs config;
	PinchReward pinchReward;

	Vec lastIntercept;

	float Corner(float x, short xOrientation, short yOrientation);
	Vec GetCornerIntersection(short xFwd, short yFwd, float xPos, float yPos);
};
END_PINCH_NS