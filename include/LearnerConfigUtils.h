#pragma once

#include <RLGymPPO_CPP/Threading/GameInst.h>
#include <RLGymPPO_CPP/LearnerConfig.h>
#include <Utils/VoidUtils.h>

#include <WandbConfig.h>

#include <yaml-cpp/yaml.h>

using namespace RLGPC;

START_VOID_NS

void OnStepSkillTracker(GameInst* gameInst, const RLGSC::Gym::StepResult& stepResult, Report& gameMetrics);
void SetupConfig(LearnerConfig& cfg, std::string configPath);

template<typename T>
T GetYamlProperty(YAML::Node node, std::string key, T defaultValue);

END_VOID_NS
