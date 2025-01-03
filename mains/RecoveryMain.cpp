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

static void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {
	BaseOnStep(gameInst, stepResult, gameMetrics, loggers);
}

static void OnIteration(Learner* learner, Report& allMetrics) {
	BaseOnIteration(learner, allMetrics, loggers);
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

