#include <LearnerConfigUtils.h>

START_VOID_NS

void OnStepSkillTracker(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics)
{
}

void SetupConfig(RLGPC::LearnerConfig& cfg, std::string configPath) {
	YAML::Node root = YAML::LoadFile(configPath);
	YAML::Node n = root["config"];

	#pragma region Process config
	YAML::Node processConfigNode = n["process-config"];
	cfg.numThreads = GetYamlProperty<int>(processConfigNode, "numThreads", 8);
	cfg.numGamesPerThread = GetYamlProperty<int>(processConfigNode, "numGamesPerThread", 16);
	#pragma endregion

	#pragma region Buffer config
	YAML::Node bufferConfigNode = n["buffer-config"];
	int tsPerItr = 200 * 1000;
	cfg.timestepsPerIteration = GetYamlProperty<int>(bufferConfigNode, "timestepsPerIteration", tsPerItr);
	cfg.expBufferSize = GetYamlProperty<int>(bufferConfigNode, "expBufferSize", tsPerItr * 3);
	#pragma endregion

	#pragma region Learner buffer config
	YAML::Node learnerBufferConfigNode = n["learner-buffer-config"];
	cfg.ppo.batchSize = GetYamlProperty<int>(learnerBufferConfigNode, "batchSize", tsPerItr);
	cfg.ppo.miniBatchSize = GetYamlProperty<int>(learnerBufferConfigNode, "miniBatchSize", 250 * 100);;
	#pragma endregion

	#pragma region Learner hyperparameters
	YAML::Node learnerHyperparametersNode = n["learner-hyperparams"];
	cfg.ppo.epochs = GetYamlProperty<int>(learnerHyperparametersNode, "epochs", 3);
	cfg.ppo.entCoef = GetYamlProperty<float>(learnerHyperparametersNode, "entCoef", 0.01f);
	cfg.ppo.policyLR = GetYamlProperty<float>(learnerHyperparametersNode, "policyLR", 8e-4);
	cfg.ppo.criticLR = GetYamlProperty<float>(learnerHyperparametersNode, "criticLR", 8e-4);
	cfg.ppo.policyLayerSizes = GetYamlProperty<std::vector<int>>(learnerHyperparametersNode, "policyLayerSizes", { 512, 512, 512 });
	cfg.ppo.criticLayerSizes = GetYamlProperty<std::vector<int>>(learnerHyperparametersNode, "criticLayerSizes", { 512, 512, 512 });
	#pragma endregion

	#pragma region Training config
	YAML::Node trainingConfigNode = n["training-config"];
	cfg.sendMetrics = GetYamlProperty<bool>(trainingConfigNode, "sendMetrics", true);
	cfg.renderMode = GetYamlProperty<bool>(trainingConfigNode, "renderMode", not cfg.sendMetrics);
	cfg.renderTimeScale = GetYamlProperty<float>(trainingConfigNode, "renderTimeScale", 1.5f);
	cfg.renderDuringTraining = GetYamlProperty<bool>(trainingConfigNode, "renderDuringTraining", false);
	cfg.deterministic = GetYamlProperty<bool>(trainingConfigNode, "deterministic", false);
	#pragma endregion

	#pragma region Checkpoints
	YAML::Node checkpointsConfigNode = n["checkpoints-config"];
	cfg.checkpointLoadFolder = GetYamlProperty<std::string>(checkpointsConfigNode, "checkpoints-load-folder", "checkpoints");
	cfg.checkpointSaveFolder = GetYamlProperty<std::string>(checkpointsConfigNode, "checkpoints-save-folder", "checkpoints");
	cfg.checkpointsToKeep = GetYamlProperty<int>(checkpointsConfigNode, "checkpoints-to-keep", 30);
	#pragma endregion

	#pragma region Wandb config
	YAML::Node wandbConfigNode = n["wandb-config"];
	cfg.metricsGroupName = GetYamlProperty<std::string>(wandbConfigNode, "metricsGroupName", WANDB_ENTITY);;
	cfg.metricsProjectName = GetYamlProperty<std::string>(wandbConfigNode, "metricsProjectName", WANDB_PROJECT);;
	cfg.metricsRunName = GetYamlProperty<std::string>(wandbConfigNode, "metricsRunName", WANDB_RUN_NAME);
	#pragma endregion

	#pragma region Skill tracker
	SkillTrackerConfig stc = {};
	YAML::Node skillTrackerConfigNode = n["skill-tracker-config"];
	stc.enabled = GetYamlProperty<bool>(skillTrackerConfigNode, "enabled", true);
	stc.stepCallback = OnStepSkillTracker;

	cfg.skillTrackerConfig = stc;
	#pragma endregion
}

float maxBallVel = 0.;

// This is our step callback, it's called every step from every RocketSim game
// WARNING: This is called from multiple threads, often simultaneously, 
//	so don't access things apart from these arguments unless you know what you're doing.
// gameMetrics: The metrics for this specific game
void BaseOnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics, std::vector<Logger*> loggers) {
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
void BaseOnIteration(Learner* learner, Report& allMetrics, std::vector<Logger*> loggers) {
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

			if (gameReport.Has(tracker.first) or gameReport.Has(tracker.first + "_avg_total"))
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

template<typename T>
inline T GetYamlProperty(YAML::Node node, std::string propertyName, T defaultValue)
{
	return node[propertyName] ? node[propertyName].as<T>() : defaultValue;
}


END_VOID_NS