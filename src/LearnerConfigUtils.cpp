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

template<typename T>
inline T GetYamlProperty(YAML::Node node, std::string propertyName, T defaultValue)
{
	return node[propertyName] ? node[propertyName].as<T>() : defaultValue;
}


END_VOID_NS