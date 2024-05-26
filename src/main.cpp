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

using namespace RLGPC; // RLGymPPO
using namespace RLGSC; // RLGymSim

std::vector<std::string> names = {
	"FaceBall",
	"VelocityPlayerToBall",
	"VelocityBallToGoal",
	"Event"
};

// This is our step callback, it's called every step from every RocketSim game
// WARNING: This is called from multiple threads, often simultaneously, 
//	so don't access things apart from these arguments unless you know what you're doing.
// gameMetrics: The metrics for this specific game
void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {

	auto& gameState = stepResult.state;
	try
	{
		((LoggedCombinedReward*)(gameInst->match->rewardFn))->LogRewards(gameMetrics);
	}
	catch (const std::exception& e)
	{
		std::cout << "Couldn't log rewards: " << e.what() << std::endl;
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
	std::vector<AvgTracker> rewards = {};

	for (int i = 0; i < names.size(); i++) {
		rewards.push_back({});
	}
	
	// Get metrics for every gameInst
	auto allGameMetrics = learner->GetAllGameMetrics();
	for (auto& gameReport : allGameMetrics) {
		avgPlayerSpeed += gameReport.GetAvg(METRICS_HEADER + std::string("player_speed"));
		avgBallTouchRatio += gameReport.GetAvg(METRICS_HEADER + std::string("ball_touch_ratio"));
		avgAirRatio += gameReport.GetAvg(METRICS_HEADER + std::string("in_air_ratio"));

		for (int i = 0; i < names.size(); i++) {
			rewards[i] += gameReport.GetAvg(REWARD_HEADER + names[i]);
		}
	}

	allMetrics[METRICS_HEADER + std::string("player_speed")] = avgPlayerSpeed.Get();
	allMetrics[METRICS_HEADER + std::string("ball_touch_ratio")] = avgBallTouchRatio.Get();
	allMetrics[METRICS_HEADER + std::string("in_air_ratio")] = avgAirRatio.Get();

	for (int i = 0; i < rewards.size(); i++) {
		allMetrics[REWARD_HEADER + names[i]] = rewards[i].Get();
	}
	std::cout << "End of iteration callback" << std::endl;
}

// Create the RLGymSim environment for each of our games
EnvCreateResult EnvCreateFunc() {
	constexpr int TICK_SKIP = 8;
	constexpr float NO_TOUCH_TIMEOUT_SECS = 3.f;

	int tsPerIter = 200 * 1000;

	auto rewards = new LoggedCombinedReward( // Format is { RewardFunc(), weight, name }
		{
			// Small reward for facing the ball
			{ new FaceBallReward(), 0.1f , names[0]},

			// Moderate reward for going towards the ball
			{ new VelocityPlayerToBallReward(), 0.5f , names[1]},

			// Bigger reward for having the ball go towards the goal
			{ new VelocityBallToGoalReward(), 1.0f , names[2]},

			// Giant reward for scoring, giant penalty for being scored on
			{ new EventReward({.teamGoal = 1.f, .concede = -1.f, .touch = 0.1f}), 50.f , names[3]},
		},
		false
	);

	std::vector<TerminalCondition*> terminalConditions = {
		new NoTouchCondition(NO_TOUCH_TIMEOUT_SECS * 120 / TICK_SKIP),
		new GoalScoreCondition()
	};

	auto obs = new DefaultOBS();
	auto actionParser = new DiscreteAction();
	auto stateSetter = new RandomState(true, true, true);

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

	// Play around with these to see what the optimal is for your machine, more isn't always better
	cfg.numThreads = 8;
	cfg.numGamesPerThread = 16;

	// We want a large itr/batch size
	// You'll want to increase this as your bot improves, up to an extent
	int tsPerItr = 200 * 1000;
	cfg.timestepsPerIteration = tsPerItr;
	cfg.ppo.batchSize = tsPerItr;
	cfg.ppo.miniBatchSize = 25 * 1000; // Lower this if too much VRAM is being allocated
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
	cfg.renderMode = false; // Don't render

	cfg.metricsGroupName = "cryy_salt";
	cfg.metricsProjectName = "cpp_loggers";
	cfg.metricsRunName = "cpp_loggers_run";

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