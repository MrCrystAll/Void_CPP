#include <RLGymPPO_CPP/Learner.h>
#include <RLGymPPO_CPP/Util/SkillTrackerConfig.h>

#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/CombinedReward.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/NoTouchCondition.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/GoalScoreCondition.h>
#include <RLGymSim_CPP/Utils/StateSetters/RandomState.h>
#include <RLGymSim_CPP/Utils/StateSetters/KickoffState.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBSPadded.h>

#include "RLBotClient.h"
#include "Utils/LoggerUtils.h"
#include "WandbConfig.h"
#include "ObsBuilder.h"

//Reward logging
#include <Logging/LoggedCombinedReward.h>
#include <Logging/TestReward.h>

#include <States.h>
#include <TerminalConditions.h>
#include <Loggers.h>
#include <LearnerConfigUtils.h>

#include <Replays/ReplayLoader.h>
#include <Replays/ReplaySetter.h>
#include <Replays/ReplayUtils.h>

#include <Replays/RCF/ReplayFilter.h>
#include <Replays/RCF/CommonRCFs.h>

#include <Utils/VoidUtils.h>
using namespace RLGPC; // RLGymPPO
using namespace RLGSC; // RLGymSim

USE_LOGGING_NS;

USE_OBS_BUILDER_NS;
USE_LOGGERS_NS;
USE_TC_NS;

USE_VOID_NS;
USE_REPLAY_NS;

USE_RCF_NS;

std::vector<Logger*> loggers = {
	//Ball Loggers
	new BallLoggers::BallSpeedLogger(),
	new BallLoggers::BallHeightLogger(),

	//Players loggers
	new PlayerLoggers::PlayerBallTouchLogger(),
	new PlayerLoggers::PlayerInAirLogger(),
	new PlayerLoggers::PlayerSpeedLogger(),
	new PlayerLoggers::PlayerHeightLogger(),
	new PlayerLoggers::FlipTimeLogger()
};

float maxBallVel = 0.;

auto stateSetter = new ReplaySetter({
		.loadExistingReplays = {true, {"Shots_Saves3v3.json"}, 0},
		.loadNewReplays = {false, {"replays"}, 30}
	});

// This is our step callback, it's called every step from every RocketSim game
// WARNING: This is called from multiple threads, often simultaneously, 
//	so don't access things apart from these arguments unless you know what you're doing.
// gameMetrics: The metrics for this specific game
void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {
	auto& gameState = stepResult.state;
	LoggedCombinedReward* lrw = dynamic_cast<LoggedCombinedReward*>(gameInst->match->rewardFn);
	if (lrw != nullptr) {
		lrw->LogAll(gameMetrics, stepResult.done);
	}

	float ballVel = gameState.ball.vel.Length();

	if (ballVel > maxBallVel) {
		maxBallVel = ballVel;
	}

	for (Logger* l : loggers) {
		l->Log(gameMetrics, gameState);
	}
}

// This is our iteration callback, it's called every time we complete an iteration, after learning
// Here we can add custom metrics to the metrics report, for example
void OnIteration(Learner* learner, Report& allMetrics) {
	std::map<std::string, AvgTracker> mTrackersAvg = {};
	std::map<std::string, float> mTrackers = {};
	std::map<std::string, AvgTracker> rTrackers = {};
	
	// Get metrics for every gameInst
	auto allGameMetrics = learner->GetAllGameMetrics();


	for (auto metric : allGameMetrics[0].data) {
		if (metric.first.starts_with(REWARD_HEADER)) {
			if (metric.first.ends_with("_avg_total")) {
				rTrackers[metric.first.substr(0, metric.first.size() - 10)] += 0;
			}
			else if (!metric.first.ends_with("_avg_count")) {
				rTrackers[metric.first] += 0;
			}
		}
	}

	for (const Logger* l : loggers) {
		for (const Metric& m : l->metrics) {
			if (m.isAvg) {
				mTrackersAvg[METRICS_HEADER + m.name] += 0;
			}
			else {
				mTrackers[METRICS_HEADER + m.name] += 0;
			}
		}
	}


	for (auto& gameReport : allGameMetrics) {
		for (auto& val : mTrackers) {
			val.second += gameReport[val.first];
		}

		for (auto& tracker : mTrackersAvg) {
			tracker.second += gameReport.GetAvg(tracker.first);
		}

		for (auto& tracker : rTrackers) {
			if(gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
			{
				tracker.second += gameReport.GetAvg(tracker.first);
			}
		}
	}

	for (const auto& tracker : rTrackers) {
		allMetrics[tracker.first] = tracker.second.Get();
	}

	for (const auto& tracker : mTrackers) {
		allMetrics[tracker.first] = tracker.second;
	}

	for (const auto& tracker : mTrackersAvg) {
		allMetrics[tracker.first] = tracker.second.Get();
	}
	
	allMetrics[METRICS_HEADER + std::string("max_ball_speed")] = maxBallVel / CommonValues::BALL_MAX_SPEED * 216;
	maxBallVel = 0;
	std::cout << "End of iteration callback" << std::endl;
}

// Create the RLGymSim environment for each of our games
EnvCreateResult EnvCreateFunc() {
	constexpr int TICK_SKIP = 4;
	constexpr float NO_TOUCH_TIMEOUT_SECS = 7.f;
	constexpr float BOUNCE_TIMEOUT_SECS = 1.f;

	/*PinchWallSetupReward::PinchWallSetupArgs args(
		{
			.creepingDistance = 2000.0f,
			.groundBanDistance = 1500.0f,
			.maxDistToTrigger = 4000.0f,
		},
		{
			.hasFlipReward = 0.8f,
			.hasFlipPunishment = -1.5f,
			.maxDistance = 250.0f
		},
		{
			.similarityBallAgentReward = 10.0f,
			.similarityBallAgentThresh = 0.9f,
			.similarityBallWallThresh = 0.9f,
		},
		{
			.groundBanPunishment = -10.0f,
			.groundBanReward = 3.0f,
			.creepingDistanceReward = 0.01f
		},
		{
			.ballDistReduction = 500.0f,
			.speedMatchW = 2.0f,
			.agentDistToBallThresh = 500.0f,
			.ballOffsetX = 200.0f,
			.ballOffsetY = 200.0f,
			.behindTheBallReward = 100.0f
		},
		{
			.wallMinHeightToPinch = 150.0f
		},
		{
			.ballHandling = {
				.ballVelW = 10000.0f,
				.touchW = 200.0f,
				.goalDirectionW = 300.0f,
				.isFlippingW = 300.0f
			}
		}
	);

	PinchCeilingSetupReward::PinchCeilingSetupArgs pinchCeilingArgs = {
		{
			.agentSimilarity =
			{
				.similarityBallAgentReward = 1.0f,
				.similarityBallAgentThresh = 0.9f,
				.speedMatchW = 0.1f
			},
			.ballGroundHandling =
			{
				.agentDistToBallThresh = 550.0f,
				.ballDistReduction = 2000.0f,
				.ballOffsetX = 250.0f,
				.ballOffsetY = 250.0f,
				.behindTheBallReward = 3.5f,
			},
			.distWallThresh = 50.0f + RLGSC::CommonValues::BALL_RADIUS,
			.groundThresh = 250.0f,
			.touchReward = 40.0f,
			.wallAgentAndBallThreshold = 0.8f,
			.wallAgentAndBallPunishment = -2.0f
		},
		{
			.ballDistReduction = 50.0f,
			.ballHeightW = 20.0f,
			.underTheBallReward = 50.0f,
			.underBallOffsetY = 350.0f
		},
		{
			.distToCeilThresh = RLGSC::CommonValues::BALL_RADIUS + 50.0f,
			.onCeilingReward = 10.0f,
			.banZoneHeight = 1500.0f,
			.groundedBan = -350.0f,
			.ungroundedReward = 60.0f
		},
		{
			.ballHandling =
			{
				.ballVelW = 20000.0f,
				.touchW = 200.0f
			}
		}
	};*/

	auto rewards = new LoggedCombinedReward( // Format is { RewardFunc, weight (optional, default = 1), name (optional for loggable rewards, mandatory for non loggable) }
		{
			{new DummyReward()}
		}
	);

	std::vector<TerminalCondition*> terminalConditions = {
		new TimeoutCondition(NO_TOUCH_TIMEOUT_SECS * 120 / TICK_SKIP),
		//new BounceTimeoutCondition(BOUNCE_TIMEOUT_SECS * 120 / TICK_SKIP),
		//new TouchTimeoutCondition(),
		new GoalScoreCondition()
	};

	auto obs = new DefaultOBSPadded(6);
	auto actionParser = new DiscreteAction();

	//auto stateSetter = new KickoffState();

	Match* match = new Match(
		rewards,
		terminalConditions,
		obs,
		actionParser,
		stateSetter,

		3, // Team size
		true // Spawn opponents
	);

	Gym* gym = new Gym(match, TICK_SKIP);
	return { match, gym };
}

int main() {
	// Initialize RocketSim with collision meshes
	RocketSim::Init("./collision_meshes");

	// Make configuration for the learner
	LearnerConfig cfg = {};

	SetupConfig(cfg);

	// Make the learner with the environment creation function and the config we just made
	Learner learner = Learner(EnvCreateFunc, cfg);

	// Set up our callbacks
	learner.stepCallback = OnStep;
	learner.iterationCallback = OnIteration;

	// Start learning!
	learner.Learn();


	//RLBotClient::Run({ .port = 23233, .obsBuilder = new DefaultOBS(), .actionParser = new DiscreteAction(), .policyPath = "checkpoints/42011264", .obsSize = 70,  .policyLayerSizes = {256, 256, 256}, .tickSkip = 8 });
	std::cout << "Learning done" << std::endl;

	return 0;
}