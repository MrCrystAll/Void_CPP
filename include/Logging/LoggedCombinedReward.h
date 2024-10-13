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

START_LOGGING_NS

using namespace RLGSC;

/// <summary>
/// A combined reward that logs every reward it has
/// </summary>
class LoggedCombinedReward: public LoggableReward
{
public:

	/// <summary>
	/// A tuple of Reward function, weight and name (name is for non-loggable)
	/// </summary>
	struct RewardArg {
		RLGSC::RewardFunction* rf;
		float w = 1.0f;
		std::string name = "";
	};

	struct RewardProp {
		LoggableReward* rf;
		float w = 1.0f;
		std::string name = "";
	};
	
	LoggedCombinedReward(std::vector<RewardArg> rewardsAndWeights, std::string name = "Logged combined reward") : LoggableReward(name) {
		for (size_t i = 0; i < rewardsAndWeights.size(); i++)
		{
			LoggableReward* temp = dynamic_cast<LoggableReward*>(rewardsAndWeights[i].rf);
			if (temp == nullptr) {
				if (rewardsAndWeights[i].name.empty()) {
					VOID_ERR("Cannot log reward number " << i + 1 << ". No name provided");
					std::exit(EXIT_FAILURE);
				}
				this->rewardsAndWeights.push_back({ new LoggableWrapper(rewardsAndWeights[i].rf, rewardsAndWeights[i].name), rewardsAndWeights[i].w, rewardsAndWeights[i].name});
			}
			else {
				this->rewardsAndWeights.push_back({temp, rewardsAndWeights[i].w, rewardsAndWeights[i].name});
			}
		}
	};
	virtual void Reset(const GameState& initialState);
	virtual void PreStep(const GameState& state);

	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);
	virtual float GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction);
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);

	virtual void LogAll(RLGPC::Report& report, bool final, std::string name = "", float weight = 1);
	virtual void PrintReward(float weight = 1.0f, bool showMedian = false, bool showStd = false, bool showMin = false, bool showMax = false);

	std::vector<RewardProp> rewardsAndWeights;
private:

};

END_LOGGING_NS
