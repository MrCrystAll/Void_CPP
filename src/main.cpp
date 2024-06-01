#include <RLGymPPO_CPP/Learner.h>

#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/CombinedReward.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/NoTouchCondition.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/GoalScoreCondition.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include <RLGymSim_CPP/Utils/StateSetters/RandomState.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

#include "RLBotClient.h"
#include "LoggedCombinedReward.h"
#include "LoggerUtils.h"
#include "WandbConfig.h"
#include <Rewards.h>
#include <States.h>
#include <TerminalConditions.h>

using namespace RLGPC; // RLGymPPO
using namespace RLGSC; // RLGymSim

std::vector<std::string> names = {
	"PinchReward"
};

float maxBallVel = 0.;

// This is our step callback, it's called every step from every RocketSim game
// WARNING: This is called from multiple threads, often simultaneously, 
//	so don't access things apart from these arguments unless you know what you're doing.
// gameMetrics: The metrics for this specific game
void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {

	auto& gameState = stepResult.state;
	try
	{
		((LoggedCombinedReward*)(gameInst->match->rewardFn))->LogRewards(gameMetrics);
		//std::cout << std::get<1>(((LoggedCombinedReward*)(gameInst->match->rewardFn))->lastRewards[0]) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Couldn't log rewards: " << e.what() << std::endl;
	}

	float ballVel = gameState.ball.vel.Length();

	gameMetrics.AccumAvg(METRICS_HEADER + std::string("ball_speed"), ballVel);
	if (ballVel > maxBallVel) {
		maxBallVel = ballVel;
	}

	for (auto& player : gameState.players) {
		// Track average player speed
		float speed = player.phys.vel.Length();
		gameMetrics.AccumAvg(METRICS_HEADER + std::string("player_speed"), speed);

		// Track ball touch ratio
		gameMetrics.AccumAvg(METRICS_HEADER + std::string("ball_touch_ratio"), player.ballTouchedStep);

		// Track in-air ratio
		gameMetrics.AccumAvg(METRICS_HEADER + std::string("in_air_ratio"), !player.carState.isOnGround);
	}
}

// This is our iteration callback, it's called every time we complete an iteration, after learning
// Here we can add custom metrics to the metrics report, for example
void OnIteration(Learner* learner, Report& allMetrics) {

	AvgTracker avgPlayerSpeed = {};
	AvgTracker avgBallTouchRatio = {};
	AvgTracker avgAirRatio = {};
	AvgTracker avgBallSpeed = {};
	std::vector<AvgTracker> rewards = {};
	std::map<std::string, AvgTracker> rTrackers = {};

	for (int i = 0; i < names.size(); i++) {
		rewards.push_back({});
	}
	
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


	for (auto& gameReport : allGameMetrics) {
		avgPlayerSpeed += gameReport.GetAvg(METRICS_HEADER + std::string("player_speed"));
		avgBallTouchRatio += gameReport.GetAvg(METRICS_HEADER + std::string("ball_touch_ratio"));
		avgAirRatio += gameReport.GetAvg(METRICS_HEADER + std::string("in_air_ratio"));
		avgBallSpeed += gameReport.GetAvg(METRICS_HEADER + std::string("ball_speed"));

		for (auto& tracker : rTrackers) {
			if(gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
			{
				tracker.second += gameReport.GetAvg(tracker.first);
			}
		}
	}


	allMetrics[METRICS_HEADER + std::string("player_speed")] = avgPlayerSpeed.Get();
	allMetrics[METRICS_HEADER + std::string("ball_touch_ratio")] = avgBallTouchRatio.Get();
	allMetrics[METRICS_HEADER + std::string("in_air_ratio")] = avgAirRatio.Get();
	allMetrics[METRICS_HEADER + std::string("ball_speed")] = avgBallSpeed.Get();


	for (auto tracker : rTrackers) {
		allMetrics[tracker.first] = tracker.second.Get();
	}


	
	allMetrics[METRICS_HEADER + std::string("max_ball_speed")] = maxBallVel / CommonValues::BALL_MAX_SPEED * 216;
	maxBallVel = 0;
	std::cout << "End of iteration callback" << std::endl;
}

// Create the RLGymSim environment for each of our games
EnvCreateResult EnvCreateFunc() {
	constexpr int TICK_SKIP = 8;
	constexpr float NO_TOUCH_TIMEOUT_SECS = 7.f;

	PinchReward::PinchArgs args(
		{
			.creepingDistance = 2000.0f,
			.groundBanDistance = 1000.0f,
			.maxDistToTrigger = 4000.0f,
		},
		{
			.hasFlipReward = 3.0f,
			.hasFlipRewardWhenBall = 20.0f,
			.hasFlipPunishment = -3.0f,
			.hasFlipPunishmentWhenBall = -20.0f,
			.maxDistance = 50.0f
		},
		{
			.similarityBallAgentReward = 0.1f,
			.similarityBallAgentThresh = 0.9f,
			.similarityBallWallThresh = 0.9f,
		},
		{
			.groundBanPunishment = -0.1f,
			.groundBanReward = 1.0f,
			.creepingDistanceReward = 0.001f
		},
		{
			.ballDistReduction = 100.0f,
			.ballVelW = 50.0f,
			.speedMatchW = 1.0f,
			.touchW = 1.0f
		},
		{
			.wallMinHeightToPinch = 150.0f
		}
	);

	auto rewards = new LoggedCombinedReward( // Format is { RewardFunc(), weight, name }
		{
			{new PinchReward(args), 1.0f, names[0]}
		},
		false
	);

	std::vector<TerminalCondition*> terminalConditions = {
		new TimeoutCondition(NO_TOUCH_TIMEOUT_SECS * 120 / TICK_SKIP),
		new GoalScoreCondition()
	};

	auto obs = new DefaultOBS();
	auto actionParser = new DiscreteAction();
	auto stateSetter = new WallPinchSetter(400.0f, 200.0f, 0.5f);

	Match* match = new Match(
		rewards,
		terminalConditions,
		obs,
		actionParser,
		stateSetter,

		1, // Team size
		false // Spawn opponents
	);

	Gym* gym = new Gym(match, TICK_SKIP);
	return { match, gym };
}

int main() {
	// Initialize RocketSim with collision meshes
	RocketSim::Init("./collision_meshes");

	// Make configuration for the learner
	LearnerConfig cfg = {};

	// Play around with these to see what the optimal is for your machine, more isn't always better
	cfg.numThreads = 8;
	cfg.numGamesPerThread = 16;

	// We want a large itr/batch size
	// You'll want to increase this as your bot improves, up to an extent
	int tsPerItr = 200 * 1000;
	cfg.timestepsPerIteration = tsPerItr;
	cfg.ppo.batchSize = tsPerItr;
	cfg.ppo.miniBatchSize = 250 * 100; // Lower this if too much VRAM is being allocated
	cfg.expBufferSize = tsPerItr * 3;
	
	// This is just set to 1 to match rlgym-ppo example
	// I've found the best value is somewhere between 2 and 4
	// Increasing this will lower SPS, but increase step efficiency
	cfg.ppo.epochs = 3; 

	// Reasonable starting entropy
	cfg.ppo.entCoef = 0.01f;

	// Decently-strong learning rate to start, may start to be too high around 100m steps
	cfg.ppo.policyLR = 8e-4;
	cfg.ppo.criticLR = 8e-4;

	// Default model size
	cfg.ppo.policyLayerSizes = { 256, 256, 256 };
	cfg.ppo.criticLayerSizes = { 256, 256, 256 };
	
	cfg.sendMetrics = true; // Send metrics
	cfg.renderMode = false; // render
	cfg.renderDuringTraining = false; //Activate that so it doesn't override

	cfg.metricsGroupName = WANDB_ENTITY;
	cfg.metricsProjectName = WANDB_PROJECT;
	cfg.metricsRunName = WANDB_RUN_NAME;

	// Make the learner with the environment creation function and the config we just made
	Learner learner = Learner(EnvCreateFunc, cfg);

	// Set up our callbacks
	learner.stepCallback = OnStep;
	learner.iterationCallback = OnIteration;


	// Start learning!
	learner.Learn();
	std::cout << "Learning done" << std::endl;

	return 0;
}