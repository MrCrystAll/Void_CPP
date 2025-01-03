//Loggers
#include <Loggers.h>
USE_LOGGERS_NS;

#include <Logging/LoggedCombinedReward.h>
USE_LOGGING_NS;

//The learner
#include <RLGymPPO_CPP/Learner.h>

//Learner config
#include <LearnerConfigUtils.h>

USE_VOID_NS;

std::vector<Logger*> loggers = {
};

static void OnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics) {
	BaseOnStep(gameInst, stepResult, gameMetrics, loggers);
}

static void OnIteration(Learner* learner, Report& allMetrics) {
	BaseOnIteration(learner, allMetrics, loggers);
}

// Create the RLGymSim environment for each of our games
EnvCreateResult EnvCreateFunc() {
	constexpr int TICK_SKIP = 8;
	constexpr float NO_TOUCH_TIMEOUT_SECS = 7.f;
	constexpr float BOUNCE_TIMEOUT_SECS = 1.f;

	auto rewards = new LoggedCombinedReward( // Format is { RewardFunc, weight (optional, default = 1), name (optional for loggable rewards, mandatory for non loggable) }
		{
			{new # Insert nameReward(), 5.0f},
		}
	);

	std::vector<TerminalCondition*> terminalConditions = {
	};

	auto obs = new # Insert nameBuilder(6);
	auto actionParser = new # Insert nameActionParser();
	auto stateSetter = new # Insert nameStateSetter();

	Match* match = new Match(
		rewards,
		terminalConditions,
		obs,
		actionParser,
		stateSetter,

		2, // Team size
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

	SetupConfig(cfg, "Configs/# Insert nameConfig.yaml");

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