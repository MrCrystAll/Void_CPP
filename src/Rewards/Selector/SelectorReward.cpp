#include "Rewards/Selector/SelectorReward.h"

float SelectorReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	std::vector<Submodel> chosenModels = this->choice->GetCurrentSubmodels();
	this->reward += {this->cbr->GetReward(player, state, prevAction), "Total without weight"};

	float weight = chosenModels[playerMapping[player.carId]].GetWeight();

	return reward.value * weight;
}

float SelectorReward::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	std::vector<Submodel> chosenModels = this->choice->GetCurrentSubmodels();
	this->reward += {this->cbr->GetFinalReward(player, state, prevAction), "Total without weight"};

	float weight = chosenModels[playerMapping[player.carId]].GetWeight();

	return reward.value * weight;
}

void SelectorReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	this->cbr->LogAll(report, final, name, weight);
	LoggableReward::LogAll(report, final, name, weight);
}

void SelectorReward::Log(Report& report, std::string name, float weight)
{
	LoggableReward::Log(report, name, weight);
	this->choice->Log(report);
}
