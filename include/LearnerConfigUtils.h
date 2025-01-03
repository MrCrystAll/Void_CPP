#pragma once

#include <RLGymPPO_CPP/Threading/GameInst.h>
#include <RLGymPPO_CPP/LearnerConfig.h>
#include <Utils/VoidUtils.h>

#include <WandbConfig.h>

#include <yaml-cpp/yaml.h>
#include <RLGymPPO_CPP/Learner.h>

#include <Logging/LoggableReward.h>
#include <Loggers.h>

using namespace RLGPC;


USE_LOGGING_NS;
USE_LOGGERS_NS;

START_VOID_NS

void OnStepSkillTracker(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics);
void SetupConfig(LearnerConfig& cfg, std::string configPath);

void BaseOnStep(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics, std::vector<Logger*> loggers);
void BaseOnIteration(Learner* learner, Report& allMetrics, std::vector<Logger*> loggers);

template<typename T>
T GetYamlProperty(YAML::Node node, std::string key, T defaultValue);

END_VOID_NS
