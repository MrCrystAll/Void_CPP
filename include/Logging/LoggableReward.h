/*****************************************************************//**
 * \file   LoggableReward.h
 * \brief  Contains the loggable reward, use it to create a new reward with loggable metrics
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once

#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include <RLGymPPO_CPP/Util/Report.h>
#include <Utils/LoggingUtils.h>

using namespace RLGSC;
using namespace RLGPC;

/**
 * @brief A reward that can log metrics and its own value
 */
class LoggableReward : public RewardFunction {
public:
	LoggedFloat reward = {};
	std::string name;

	/**
	 * @brief Logs everything.
	 * 
	 * \param report The report to log to
	 * \param final Whether the state is final
	 * \param name The name you want to save it to (empty if none)
	 * \param weight The weight of the reward
	 */
	virtual void LogAll(Report& report, bool final, std::string name, float weight);

	/**
	 * @brief Logs if final
	 * 
	 * \param report The report to log to
	 * \param name The name you want to save it to (empty if none)
	 * \param weight The weight of the reward
	 */
	virtual void LogFinal(Report& report, std::string name, float weight);

	/**
	 * @brief Logs if not final
	 * 
	 * \param report The report to log to
	 * \param name The name you want to save it to (empty if none)
	 * \param weight The weight of the reward
	 */
	virtual void Log(Report& report, std::string name, float weight);

	/**
	 * @brief Resets the reward.
	 * 
	 * \param initialState The state usable to setup your reward for the current episode
	 */
	virtual void Reset(const GameState& initialState);

	/**
	 * @brief Computes stuff before stepping
	 * 
	 * \param state The state you'll calculate the reward on
	 */
	virtual void PreStep(const GameState& state);

	/**
	 * {brief Print reward and metrics.
	 * 
	 * \param weight The weight of the reward
	 * \param showMedian Whether to print median
	 * \param showStd Whether to print standard deviation
	 * \param showMin Whether to print minimum
	 * \param showMax Whether to print maximum
	 */
	virtual void PrintReward(float weight = 1.0f, bool showMedian = false, bool showStd = false, bool showMin = false, bool showMax = false);

	LoggableReward(std::string name) : name(name) {};

	/**
	 * @brief Get all rewards for all players
	 * 
	 * \param state The state to get rewards
	 * \param prevActions Previous actions
	 * \param final Whether the state is final
	 * \return A list of all the rewards
	 */
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);
};

class LoggableWrapper : public LoggableReward {
public:
	/**
	 * @brief Resets the reward.
	 *
	 * \param initialState The state usable to setup your reward for the current episode
	 */
	virtual void Reset(const GameState& initialState);

	/**
	 * @brief Computes stuff before stepping
	 *
	 * \param state The state you'll calculate the reward on
	 */
	virtual void PreStep(const GameState& state);

	LoggableWrapper(RewardFunction* rfn, std::string name) : LoggableReward(name), rfn(rfn) {};

	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);

	virtual float GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction);
private:
	RewardFunction* rfn;
};
