//Recovery stuff
#include <Recovery/RecoveryUtils.h>

#include <Recovery/ActionParsers/RecoveryActionParser.h>

#include <Recovery/StateSetters/AboveGroundState.h>
#include <Recovery/StateSetters/RecoveryStateSetter.h>

#include <Recovery/Rewards/RecoveryReward.h>
#include <Recovery/Rewards/IsFlippingReward.h>
#include <Recovery/Rewards/TimeBetweenFlipsPunishment.h>

#include <Recovery/ObsBuilders/RecoveryObsBuilder.h>
#include <Recovery/ObsBuilders/DashObsBuilder.h>

#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>

//Loggers
#include <Loggers.h>
USE_LOGGERS_NS;

#include <Replays/ReplaySetter.h>
USE_REPLAY_NS;

//The learner
#include <RLGymPPO_CPP/Learner.h>

//Logging
#include <Logging/LoggedCombinedReward.h>
#include <Logging/TestReward.h>
USE_LOGGING_NS;

//TC
#include <RLGymSim_CPP/Utils/TerminalConditions/GoalScoreCondition.h>
#include <TerminalConditions.h>
USE_TC_NS;

//Learner config
#include <LearnerConfigUtils.h>
#include <WandbConfig.h>

//Uses (Auto-generated comment)
USE_RECOVERY_REWARDS_NS;
USE_RECOVERY_STATES_NS;
USE_RECOVERY_AP_NS;
USE_RECOVERY_OB_NS;

USE_VOID_NS;

std::vector<Logger*> loggers = {
	//Ball Loggers
	new BallLoggers::BallSpeedLogger(),
	new BallLoggers::BallHeightLogger(),

	//Players loggers
	new PlayerLoggers::PlayerBallTouchLogger(),
	new PlayerLoggers::PlayerInAirLogger(),
	new PlayerLoggers::PlayerSpeedLogger(),
	new PlayerLoggers::PlayerHeightLogger(),
	new PlayerLoggers::FlipTimeWhenAboveZeroLogger(),
	new PlayerLoggers::HasJumpedNotFlippedLogger()
};

float maxBallVel = 0.;

// This is our step callback, it's called every step from every RocketSim game
// WARNING: This is called from multiple threads, often simultaneously, 
//	so don't access things apart from these arguments unless you know what you're doing.
// gameMetrics: The metrics for this specific game
void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {
	auto& gameState = stepResult.state;
	LoggableReward* lrw = dynamic_cast<LoggableReward*>(gameInst->match->rewardFn);
	if (lrw != nullptr) {
		lrw->LogAll(gameMetrics, stepResult.done, "", 1.0f);
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

	VOID_LOG("Creating keys...");

	for (auto& [key, val] : allGameMetrics[0].data) {
		if (key.starts_with(REWARD_HEADER)) {
			if (key.ends_with("_avg_total")) {
				rTrackers[key.substr(0, key.size() - 10)] += val / allGameMetrics[0].data[key.substr(0, key.size() - 10) + "_avg_count"];
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
	VOID_LOG("Calculating values...");


	for (int i = 1; i < allGameMetrics.size(); i++) {
		Report& gameReport = allGameMetrics[i];

		for (auto& val : mTrackers) {
			val.second += gameReport[val.first];
		}

		for (auto& tracker : mTrackersAvg) {
			
			if(gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
			{
				tracker.second += gameReport.GetAvg(tracker.first);
			}
		}

		for (auto& tracker : rTrackers) {
			if (gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
			{
				tracker.second += gameReport.GetAvg(tracker.first);
			}
		}
	}

	VOID_LOG("Creating final report...");

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
	constexpr int TICK_SKIP = 2;
	constexpr float NO_TOUCH_TIMEOUT_SECS = 7.f;
	constexpr float BOUNCE_TIMEOUT_SECS = 1.f;

	auto rewards = new LoggedCombinedReward( // Format is { RewardFunc, weight (optional, default = 1), name (optional for loggable rewards, mandatory for non loggable) }
		{
			{new VelocityPlayerToBallReward(), 1.5f, "Velocity player to ball" },
			{new RecoveryReward({.distanceToBallWeight = 0.0f, .flipTimeWeight = 1.0f, .velocityWeight = 10.0f,  .doubleJumpWeight = 1.0f, .facingUpWeight = 10.0f, .dashResultWeight = 1000.0f}), 1.0f},
			{new EventReward({.touch = 10.0f}), 1.0f, "Event"}
		}
	);

	std::vector<TerminalCondition*> terminalConditions = {
		new TimeoutCondition(NO_TOUCH_TIMEOUT_SECS * 120 / TICK_SKIP),
		//new BounceTimeoutCondition(BOUNCE_TIMEOUT_SECS * 120 / TICK_SKIP),
		new GoalScoreCondition()
	};

	auto obs = new DashObsBuilder(1);
	auto actionParser = new RecoveryActionParser();
	auto stateSetter = new RandomRecoveryState();

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

	SetupConfig(cfg, "Configs/RecoveryConfig.yaml");

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

