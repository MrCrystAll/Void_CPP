#pragma once
#include <Logging/LoggableReward.h>
#include <Logging/LoggedCombinedReward.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include <Utils/Selector/SelectorChoice.h>
#include <map>
#include <vector>

class SelectorReward : public LoggableReward {
public:
	SelectorReward(LoggedCombinedReward* cbr, SelectorChoice* choice, std::string name = "Selector reward") : choice(choice), LoggableReward(name), cbr(cbr) {};

	SelectorChoice* choice;
	std::map<int, int> playerMapping = {
		{1, 0},
		{2, 1},
		{3, 2},
		{5, 3},
		{6, 4},
		{7, 5}
	};

	float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual float GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction);

	virtual void LogAll(Report& report, bool final, std::string name, float weight);
	virtual void Log(Report& report, std::string name, float weight);


private:
	LoggedCombinedReward* cbr;
};