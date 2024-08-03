#include <Selector/SelectorUtils.h>
#include <Selector/Rewards/SelectorReward.h>

USE_SELECTOR_REWARDS_NS;

float SelectorReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	std::vector<Submodel> chosenModels = this->choice->GetCurrentSubmodels();
	this->reward += {this->cbr->GetReward(player, state, prevAction), "Total without weight"};
	this->reward *= {chosenModels[playerMapping[player.carId]].GetWeight(), "Model choice"};

	return this->ComputeReward();
}

float SelectorReward::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	std::vector<Submodel> chosenModels = this->choice->GetCurrentSubmodels();
	this->reward += {this->cbr->GetFinalReward(player, state, prevAction), "Total without weight"};
	this->reward *= {chosenModels[playerMapping[player.carId]].GetWeight(), "Model choice"};

	return this->ComputeReward();
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
