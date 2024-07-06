/*****************************************************************//**
 * \file   LoggedCombinedReward.h
 * \brief  Contains the combined reward used to work with multiple loggable rewards
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once
#include <vector>
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include <Logging/LoggableReward.h>
#include <RLGymPPO_CPP/Lists.h>
#include <RLGymPPO_CPP/Util/Report.h>
#include <Utils/LoggerUtils.h>

using namespace RLGSC;

class LoggedCombinedReward: public RLGSC::RewardFunction
{
public:
	LoggedCombinedReward(std::vector<std::pair<RLGSC::RewardFunction*, float>> rewardsAndWeights) : rewardsAndWeights(rewardsAndWeights) {};
	virtual void Reset(const GameState& initialState);
	virtual void PreStep(const GameState& state);

	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);
	virtual float GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction);
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);

	virtual void LogAll(RLGPC::Report& report, bool final, std::string name = "", float weight = 1);
	virtual void PrintRewards(bool showMedian = false, bool showStd = false, bool showMin = false, bool showMax = false);

	std::vector<std::pair<RLGSC::RewardFunction*, float>> rewardsAndWeights;
private:

};