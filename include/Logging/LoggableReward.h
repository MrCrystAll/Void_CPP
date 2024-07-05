#pragma once

#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include <RLGymPPO_CPP/Util/Report.h>
#include <Utils/LoggingUtils.h>

using namespace RLGSC;
using namespace RLGPC;

class LoggableReward : public RewardFunction {
public:
	LoggedFloat reward = {};
	std::string name;

	virtual void LogAll(Report& report, bool final, std::string name, float weight);
	virtual void LogFinal(Report& report, std::string name, float weight);
	virtual void Log(Report& report, std::string name, float weight);


	virtual void Reset(const GameState& initialState);
	virtual void PreStep(const GameState& state);

	virtual void PrintReward(float weight = 1.0f, bool showMedian = false, bool showStd = false, bool showMin = false, bool showMax = false);

	LoggableReward(std::string name) : name(name) {};

	// Get all rewards for all players
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);
};