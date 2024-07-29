#include <CeilingPinch/CeilingPinchUtils.h>
#include <Logging/LoggableReward.h>
#include <Pinch/Rewards/PinchReward.h>

using namespace RLGSC;

START_CEILINGPINCH_REWARDS_NS
USE_PINCH_REWARDS_NS;

class CeilingPinchReward: public LoggableReward{
public:

	struct BallGroundHandling {
		// Distance from where the bot is considered too far to be behind the ball
		float agentDistToBallThresh = 550.0f;

		//Ball distance punishment reduction
		float ballDistReduction = 1000.0f;

		//Allowed offset off the ball on the x axis
		float ballOffsetX = 150.0f;

		//Allowed offset off the ball on the y axis
		float ballOffsetY = 150.0f;

		//Reward for being behind the ball
		float behindTheBallReward = 0.01f;


	};

	struct AgentSimilarity {

		//Reward for being above similarityBallAgentThresh (matching ball direction)
		float similarityBallAgentReward = 0.1f;

		//Min similarity between agent and ball's normalized velocities
		float similarityBallAgentThresh = 0.8f;

		//Weight of the reward for the agent matching ball speed
		float speedMatchW = 1.0f;
	};

	struct CeilingHandling {
		//Distance from ceiling where considered to correct height (generally one ball away from ceiling)
		float distToCeilThresh = RLGSC::CommonValues::BALL_RADIUS + 20;

		//Distance for being on the "ceiling zone"
		float onCeilingReward = 0.01f;

		//Height from which being grounded is banned
		float banZoneHeight = 1500.0f;

		//Punishment for being grounded in the ground ban zone
		float groundedBan = -1.0f;

		//Punishment for being grounded in the ground ban zone
		float ungroundedReward = 1.0f;
	};

	struct GroundHandling {
		//Agent similarity options
		AgentSimilarity agentSimilarity = {};

		//Ball ground handling options
		BallGroundHandling ballGroundHandling = {};

		//Distance from the wall to be away from it
		float distWallThresh = 50.0f + RLGSC::CommonValues::BALL_RADIUS;

		//Height from where you are considered off ground
		float groundThresh = 200.0f;

		//Touch reward
		float touchReward = 1.0f;

		//Ball and agent limit similarity to wall
		float wallAgentAndBallThreshold = 0.5f;

		//Ball and agent punishment for not similarity to wall
		float wallAgentAndBallPunishment = -2.0f;
	};

	struct WallHandling {
		//Ball distance punishment reduction (on wall)
		float ballDistReduction = 500.0f;

		//Ball height weight
		float ballHeightW = 15.0f;

		//Reward for being under the ball
		float underTheBallReward = 10.0f;

		//Offset of Y under the ball
		float underBallOffsetY = 100.0f;
	};

	struct PinchCeilingSetupArgs {
		GroundHandling groundHandling = {};
		WallHandling wallHandling = {};
		CeilingHandling ceilingHandling = {};
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	CeilingPinchReward(PinchCeilingSetupArgs args, std::string name = "Ceiling pinch reward") : config(args), LoggableReward(name), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);
private:
	PinchCeilingSetupArgs config;
	PinchReward pinchReward;
};

END_CEILINGPINCH_NS
