//Loggers
#include <Loggers.h>
USE_LOGGERS_NS;

//The learner
#include <RLGymPPO_CPP/Learner.h>

//Learner config
#include <LearnerConfigUtils.h>

USE_VOID_NS;

std::vector<Logger*> loggers = {
};

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
			if (gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
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

	std::cout << "End of iteration callback" << std::endl;
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